/*
 * A340161  a(n) is the smallest number k for which the set {k + 1, k + 2,
 *          ..., k + k} contains exactly n elements with exactly three 1-bits
 *          (A014311).
 * https://oeis.org/A340161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A018900(n) + 1 (A018900 gives the value of k−1)
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return nCr for n ≤ 63 and r ∈ {0,1,2,3}.  */
static inline unsigned long long
comb(unsigned long long n, unsigned r)
{
    if (r == 0) return 1ULL;
    if (r == 1) return n;
    if (r == 2) return n * (n - 1) / 2ULL;
    if (r == 3) return n * (n - 1) * (n - 2) / 6ULL;
    return 0ULL;
}

/* Count how many integers x (0 ≤ x ≤ N) have exactly three 1‑bits. */
static inline unsigned long long
count3_leq_uint64(unsigned long long N)
{
    unsigned long long cnt = 0ULL;
    unsigned rem  = 3; /* how many 1-bits we still need */

    for (int bit = 63; bit >= 0; --bit) {
        if ((N >> bit) & 1ULL) {
            if (rem <= (unsigned)bit) {
                cnt += comb(bit, rem);
            }
            if (rem == 0) break;
            --rem;
            if (rem == 0) break;  /* we can stop early if we already matched 3 bits */
        }
    }
    if (rem == 0) cnt += 1ULL; /* include N itself if it has exactly 3 ones */
    return cnt;
}

/* Number of 3‑bit integers inside the interval (k,2k] */
static inline unsigned long long
count_interval_uint64(unsigned long long k)
{
    return count3_leq_uint64(k << 1) - count3_leq_uint64(k);
}

/* External function requested by the problem statement. */
seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0) return (seq_t)1;

    /* target count of 3‑bit numbers we want in the interval */
    unsigned long long target = (unsigned long long)n;

    /* Find an upper bound for k.  Start at 1 and double until the count
       reaches or exceeds the target. */
    unsigned long long high = 1ULL;
    while (count_interval_uint64(high) < target) {
        high <<= 1ULL;
    }

    /* Binary search for the smallest k with count >= target.
       count_interval(0) = 0, so low can start at 0. */
    unsigned long long low = 0ULL;
    while (low + 1 < high) {
        unsigned long long mid = low + (high - low) / 2ULL;
        if (count_interval_uint64(mid) >= target) {
            high = mid;
        } else {
            low = mid;
        }
    }
    return (seq_t)high;
}
