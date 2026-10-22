/*
 * A366979  Number of divisors of n less than or equal to d(n).
 * https://oeis.org/A366979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ d : d | n AND d <= d(n) }, with d(n) = τ(n)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive divisor counter (uses idx_t for limits, seq_t for counts) */
static void
count_divs(const idx_t *primes, const int *exps, int pc,
           int idx, idx_t cur, idx_t limit, seq_t *cnt)
{
    if (idx == pc) {                 /* all prime factors processed */
        if (cur <= limit)
            (*cnt)++;
        return;
    }

    idx_t p    = primes[idx];
    idx_t val  = cur;
    int max_e  = exps[idx];

    for (int e = 0; e <= max_e; ++e) {
        count_divs(primes, exps, pc, idx + 1, val, limit, cnt);
        /* stop if next multiplication would exceed limit */
        if (e == max_e)
            break;
        if (val > limit / p)         /* avoid overflow and useless work */
            break;
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for n=0, but return 0 safely */
        return 0;

    /* --- prime factorisation of n (trial division) --- */
    idx_t tmp = n;
    idx_t primes[64];
    int  exps[64];
    int  pc = 0;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc]  = e;
            ++pc;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        primes[pc] = tmp;
        exps[pc]   = 1;
        ++pc;
    }

    /* --- compute d(n) = τ(n) --- */
    idx_t d = 1;
    for (int i = 0; i < pc; ++i)
        d *= (idx_t)(exps[i] + 1);

    /* --- count divisors <= d(n) --- */
    seq_t cnt = 0;
    count_divs(primes, exps, pc, 0, (idx_t)1, d, &cnt);

    return cnt;
}
