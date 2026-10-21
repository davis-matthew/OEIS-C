/*
 * A280055  Nachos sequence based on 1 plus primes (A008578).
 * https://oeis.org/A280055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps required to reduce n to 0 by repeatedly
 *            subtracting the largest element of the set S = { 1, 1+2, 1+2+3,
 *            1+2+3+5, … } (i.e. 1 plus the cumulative sum of the prime
 *            numbers). This implements the greedy subtraction described in
 *            the OEIS entry for A280055
 * Range:     u8 n <= 370, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_S 2000   /* sufficient for n ≤ 2⁶⁴ – values grow quickly */

seq_t a(idx_t n)
{
    /* Convert the index to a seq_t for all further arithmetic. */
    seq_t n_seq = (seq_t)n;
    if (n_seq == 0)   /* no defined values for n = 0, but return 0 */
        return (seq_t)0;

    /* Build the subtraction set S = {1} ∪ {1 + sum of the first k primes}.
       Only the elements ≤ n are required. */
    static seq_t S[MAX_S];
    size_t S_len = 0;
    S[S_len++] = (seq_t)1;   /* the fixed element 1 */

    /* Prime generation up to the point where the cumulative sum exceeds n. */
    uint64_t primes[200];     /* more than enough for n up to 2⁶⁴ */
    size_t primes_cnt = 0;
    uint64_t sum = 0;         /* sum of primes encountered so far */

    for (uint64_t p = 2; ; ++p) {
        /* Simple primality test using already found primes. */
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t pr = primes[i];
            if (pr > p / pr) break;      /* pr*pr > p, stop testing */
            if (p % pr == 0) { is_prime = false; break; }
        }
        if (!is_prime)
            continue;

        /* Record prime and update cumulative sum. */
        primes[primes_cnt++] = p;
        sum += p;
        seq_t val = (seq_t)(sum + 1);
        if (val > n_seq)   /* all remaining elements would be too large */
            break;
        if (S_len >= MAX_S)   /* safety guard */
            break;
        S[S_len++] = val;
    }

    /* Greedy subtraction: repeatedly subtract the largest usable element. */
    seq_t remaining = n_seq;
    seq_t steps = 0;

    while (remaining > 0) {
        /* Binary search for the largest S[i] ≤ remaining. */
        size_t lo = 0;
        size_t hi = S_len;   /* hi is exclusive */
        while (lo < hi) {
            size_t mid = lo + (hi - lo) / 2;
            if (S[mid] <= remaining)
                lo = mid + 1;
            else
                hi = mid;
        }
        size_t idx = lo - 1;   /* guaranteed to exist because S[0] == 1 */

        remaining -= S[idx];
        ++steps;
    }

    return steps;   /* steps already of type seq_t */
}
