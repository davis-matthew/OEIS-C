/*
 * A244259  Number of non-twin isolated divisors of n.
 * https://oeis.org/A244259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  Formula:  a(n) = |{ d | d divides n,
 *             and  none of d-2, d-1, d+1, d+2 (if positive) divides n }|
 *
 *  The function enumerates all divisors of n by first factoring
 *  n and then generating all products of its prime powers.
 *  Each divisor d is tested against the four possible neighbours.
 *
 *  Uses only the standard C types and the types seq_t/idx_t
 *  defined by the build.  No external libraries or special
 *  helper functions are required.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Recursive helper to enumerate divisors.
 * primes[pcnt] / exps[pcnt] contain the prime factorisation.
 * idx is the current position in the factor arrays.
 * cur stores the divisor being built.
 */
static void rec_divs(const idx_t primes[], const unsigned exps[],
                     size_t pcnt, size_t idx, idx_t cur, idx_t n,
                     seq_t *cnt)
{
    if (idx == pcnt) {
        /* d is 'cur' */
        bool ok = true;

        /* d-2 must not divide n, only consider if d > 2 (positive) */
        if (cur > (idx_t)2 && n % (cur - (idx_t)2) == (idx_t)0)
            ok = false;
        /* d-1 */
        if (ok && cur > (idx_t)1 && n % (cur - (idx_t)1) == (idx_t)0)
            ok = false;
        /* d+1 */
        if (ok && n % (cur + (idx_t)1) == (idx_t)0)
            ok = false;
        /* d+2 */
        if (ok && n % (cur + (idx_t)2) == (idx_t)0)
            ok = false;

        if (ok)
            ++(*cnt);
        return;
    }

    idx_t pr = primes[idx];
    idx_t cur_val = cur;
    for (unsigned e = 0; e <= exps[idx]; ++e) {
        rec_divs(primes, exps, pcnt, idx + 1, cur_val, n, cnt);
        cur_val *= pr;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Undefined for n=0, but harmless. */
    if (n == 0)
        return 0;

    /* Factorise n by simple trial division. */
    idx_t tmp = n;
    idx_t primes[64];
    unsigned exps[64];
    size_t pcnt = 0;

    /* handle factor 2 separately */
    if ((tmp & (idx_t)1) == 0) {
        unsigned e = 0;
        while ((tmp & (idx_t)1) == 0) {
            tmp >>= 1;
            ++e;
        }
        primes[pcnt] = 2;
        exps[pcnt]   = e;
        ++pcnt;
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }

    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    seq_t count = 0;
    rec_divs(primes, exps, pcnt, 0, (idx_t)1, n, &count);
    return count;
}
