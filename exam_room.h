class ExamRoom {
public:
    struct Range {
        int r;
        std::pair<int, int> ends;
        bool operator<(const Range& rhs) const {
            if (r != rhs.r) return rhs.r < r;
            return ends.first < rhs.ends.first;
        }
        bool operator>(const Range& rhs) const {
            return rhs.operator<(*this);
        }
    };
    using Ranges = std::set<Range>;
    Ranges ranges_;
    std::map<int, Ranges::iterator> seats_;
    int limit_;
    
    ExamRoom(int N)
    : limit_(N)
    {}
    
    int seat() {
        if (!limit_ || (ranges_.size() == limit_)) {
            return -1;
        }
        if (ranges_.empty()) {
            auto itr = ranges_.insert({ limit_ - 2, { 0, limit_ - 1 } }).first;
            seats_.insert({ 0, itr });
            return 0;
        } else {
            auto largest_range_itr = ranges_.begin();
            auto range = largest_range_itr->r;
            if (range == -1) return -1;
            auto first = largest_range_itr->ends.first;
            auto second = largest_range_itr->ends.second;
            
            auto mid = first + (range + 1);
            if (first == -1) mid = 0;
            else if (second == limit_ - 1) mid = limit_ - 1;
            ranges_.erase(largest_range_itr);
            
            // Insert  mid-first-1, first, mid-1; and range- mid, second
            if (mid != 0) {
                auto upper_range = (range > 0) ? (mid-first-2)/2 : -1; //
                auto r1 = ranges_.insert({ upper_range,{ first, mid - 1 } }).first;
                seats_[first] = r1;
            } else {
                seats_.erase(first);
            }
            // Second half
            auto second_range = (mid == second) ? -1 : (second - mid - 1)/2;
            auto r2 = ranges_.insert({ second_range,{ mid, second } }).first;
            seats_.insert({mid, r2});
            return mid;
        }
    }

    void leave(int p) {
        auto seat_itr = seats_.find(p);
        if (seat_itr == seats_.end()) return;
        //-----------------------------------
        int prev_seat = -1;
        if (seat_itr != seats_.begin() && !seats_.empty()) {
            auto prev_itr = std::prev(seat_itr)->second;
            prev_seat = prev_itr->ends.first;
            ranges_.erase(prev_itr);
        }
        auto range_itr = seat_itr->second;
        auto cur_last = range_itr->ends.second;
        ranges_.erase(range_itr);
        seats_.erase(p);
        //------------------------------------
        if (prev_seat == -1 && cur_last == limit_ - 1) {
            seats_.clear();
            return;
        }
        int new_range = (cur_last - prev_seat - 1) / 2;
        if (prev_seat == -1) new_range = cur_last;
        else if (cur_last == limit_ - 1) new_range = cur_last - prev_seat - 1;
        auto new_range_itr = ranges_.insert({ new_range, { prev_seat, cur_last } }).first;
        seats_[prev_seat] = new_range_itr;
    }
};

/**
 * Your ExamRoom object will be instantiated and called as such:
 * ExamRoom* obj = new ExamRoom(N);
 * int param_1 = obj->seat();
 * obj->leave(p);
 */