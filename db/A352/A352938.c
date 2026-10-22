/*
 * A352938  Irregular table T(n, k), n >= 0, k = 1..A080100(n), read by rows:
 *          the n-th row contains in ascending order the distinct nonnegative
 *          integers k <= n that have no common 1-bit with n.
 * https://oeis.org/A352938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look, base
 * Formula:   a(n) = flattened list of the irregular table T(n,k)
 * Range:     u8 n <= 9841, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int floor_log2_uint64(uint64_t x) {
    return 63 - __builtin_clzll(x);
}

static inline uint64_t popcount_uint64(uint64_t x) {
    return __builtin_popcountll(x);
}

/*
 * Precompute 3^i for i = 0..64 as unsigned __int128 to avoid overflow.
 * The table is initialized on first use.
 */
static unsigned __int128 pow3_table[65];
static int pow3_inited = 0;
static void init_pow3(void) {
    if (pow3_inited) return;
    pow3_inited = 1;
    pow3_table[0] = 1;
    for (int i = 1; i <= 64; ++i) {
        pow3_table[i] = pow3_table[i - 1] * 3;
    }
}

/* Sum of L(i) for i = 0..R, saturated at (n+1) for binary search */
static int sum_greater_equal_n(uint64_t R, uint64_t n) {
    init_pow3();
    unsigned __int128 target = (unsigned __int128)n + 1;
    if (R == 0) {
        if ((unsigned __int128)1 >= target) return 1;
        return 0;
    }
    int m = floor_log2_uint64(R);
    uint64_t block_start = 1ULL << m;
    unsigned __int128 sum = 0;
    /* Sum for all full blocks up to 2^m - 1 */
    unsigned __int128 block_sum = (pow3_table[m] + 1) >> 1;
    if (block_sum >= target) return 1;
    sum += block_sum;
    if (R == block_start - 1) {
        return sum < target ? 0 : 1;
    }
    /* Partial block for indices [2^m, R] */
    uint64_t X = R - block_start;
    /* Partial sum calculation as described */
    int zeros = 0;
    for (int pos = m - 1; pos >= 0; --pos) {
        if (X & (1ULL << pos)) {
            unsigned __int128 block = pow3_table[pos] * ((unsigned __int128)1 << (zeros + 1));
            sum += block;
            if (sum >= target) return 1;
        } else {
            zeros++;
        }
    }
    unsigned int pc = popcount_uint64(X);
    unsigned __int128 weight = ((unsigned __int128)1 << (m - pc));
    sum += weight;
    return sum >= target ? 1 : 0;
}

/* Exact sum of L(i) for i = 0..R, result fits in 64 bits when R <= n-1 */
static unsigned __int128 sum_leq(uint64_t R) {
    init_pow3();
    if (R == 0) return 1;
    int m = floor_log2_uint64(R);
    uint64_t block_start = 1ULL << m;
    unsigned __int128 sum = 0;
    /* Sum for all full blocks up to 2^m - 1 */
    unsigned __int128 block_sum = (pow3_table[m] + 1) >> 1;
    sum += block_sum;
    if (R == block_start - 1) return sum;
    /* Partial block for indices [2^m, R] */
    uint64_t X = R - block_start;
    int zeros = 0;
    for (int pos = m - 1; pos >= 0; --pos) {
        if (X & (1ULL << pos)) {
            unsigned __int128 block = pow3_table[pos] * ((unsigned __int128)1 << (zeros + 1));
            sum += block;
        } else {
            zeros++;
        }
    }
    unsigned int pc = popcount_uint64(X);
    unsigned __int128 weight = ((unsigned __int128)1 << (m - pc));
    sum += weight;
    return sum;
}

/* Length L(r) of row r */
static inline uint64_t row_length(uint64_t r) {
    if (r == 0) return 1;
    int m = floor_log2_uint64(r);
    return 1ULL << (m - popcount_uint64(r) + 1);
}

seq_t a(idx_t n) {
    init_pow3();
    /* Find the smallest r such that cum_leq(r) > n */
    uint64_t lo = 0;
    uint64_t hi = n;   /* cum_leq(r) >= r+1, so r <= n for cum_leq(r) > n */
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (sum_greater_equal_n(mid, n))
            hi = mid;
        else
            lo = mid + 1;
    }
    uint64_t r = lo;
    if (r == 0) return (seq_t)0;
    /* cumulative sum before row r */
    unsigned __int128 cum_before = sum_leq(r - 1);
    uint64_t idx_in_row = (uint64_t)n - (uint64_t)cum_before;
    /* Compute the k-th element within row r */
    int m = floor_log2_uint64(r);
    uint64_t val = 0;
    uint64_t mask = 1;
    uint64_t bit_index = 0;
    for (int bit = 0; bit < m; ++bit) {
        if ((r & mask) == 0) {          /* zero bit position at this place */
            if (idx_in_row & (1ULL << bit_index))
                val |= mask;
            ++bit_index;
        }
        mask <<= 1;
    }
    return (seq_t)val;
}
