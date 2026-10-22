/*
 * A348337  For n >= 1; x = n, then iterate x --> x + d(x) until d(x + d(x))
 *          >= d(x). a(n) gives the number of iteration steps where d(i) is
 *          the number of divisors of i, A000005(i).
 * https://oeis.org/A348337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps of the process start = n, repeat x → x +
 *            d(x) while d(x + d(x)) ≥ d(x), where d(m) is the number of
 *            divisors of m (A000005)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ------------------------------------------------------------------
 *  Small divisor-count routine for uint64_t arguments.
 *  It works for any n ≤ 2^64−1, which suffices for this sequence
 *  because the sequence values start at n ≤ 2^64−1 and grow only
 *  by adding d(x) (a tiny number) each step.
 * ------------------------------------------------------------------ */
static uint64_t divisor_count_u64(uint64_t n)
{
    if (n <= 1) return 1;   /* d(0) is not needed, but d(1)=1 */
    uint64_t result = 1;
    uint64_t temp   = n;

    /* factor 2 separately */
    if ((temp & 1ULL) == 0) {
        uint64_t exp = 0;
        do {
            temp >>= 1;
            ++exp;
        } while ((temp & 1ULL) == 0);
        result *= (exp + 1);
    }

    /* odd factors */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            do {
                temp /= p;
                ++exp;
            } while (temp % p == 0);
            result *= (exp + 1);
        }
    }

    if (temp > 1)            /* remaining prime factor */
        result *= 2;

    return result;
}

/* ------------------------------------------------------------------
 *  Main sequence function.
 *  No caching is used to keep the code portable to both C and C++.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* out of range – not used */

    uint64_t x = n;          /* current value (fits in 64‑bit) */
    seq_t   steps = 0;       /* number of iterations (seq_t type) */

    for (;;) {
        ++steps;
        uint64_t d_cur = divisor_count_u64(x);
        uint64_t next  = x + d_cur;
        uint64_t d_next = divisor_count_u64(next);
        if (d_next < d_cur) break;
        x = next;
    }

    return steps;
}
