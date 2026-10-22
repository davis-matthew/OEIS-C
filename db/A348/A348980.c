/*
 * A348980  a(n) = Sum_{d|n} d * A322582(n/d), where A322582(n) = n -
 *          A003958(n), and A003958 is fully multiplicative with a(p) = (p-1).
 * https://oeis.org/A348980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d * (n/d - A003958(n/d)), where A003958 is
 *            multiplicative with A003958(p^e) = (p-1)^e
 * Range:     u8 n <= 47, u16 n <= 2159, u32 n <= 9217, u64 n <= 9217,
 *            u128 n <= 9217, big any n
 * Helpers:   compute_G
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/compute_G.h"

/* Generate all positive divisors of n. The resulting array 'divs'
   is filled with all divisors, and '*divc' is set to the count. */
static void generate_divisors(idx_t n, idx_t divs[], size_t *divc)
{
    /* First factor n. */
    idx_t primes[16];
    int    exps[16];
    size_t pcnt = 0;
    idx_t t = n;

    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]  = e;
            ++pcnt;
        }
    }
    if (t > 1) {
        primes[pcnt] = t;
        exps[pcnt]  = 1;
        ++pcnt;
    }

    /* Build divisors iteratively. */
    divs[0] = 1;
    *divc   = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        idx_t p   = primes[i];
        int  e    = exps[i];
        size_t cur_cnt = *divc;
        idx_t p_pow = p;   /* p^1 */

        for (int exp = 1; exp <= e; ++exp) {
            for (size_t j = 0; j < cur_cnt; ++j) {
                divs[*divc] = divs[j] * p_pow;
                ++(*divc);
            }
            if (exp < e) p_pow *= p;
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety guard – offset is 1 */

    /* Estimate maximum number of divisors for n <= 10000 is < 200. */
    idx_t  divs[256];
    size_t divc;
    generate_divisors(n, divs, &divc);

    seq_t result = (seq_t)0;
    for (size_t i = 0; i < divc; ++i) {
        idx_t d   = divs[i];
        idx_t m   = n / d;
        seq_t g   = compute_G(m);
        result  += (seq_t)d * g;
    }
    return result;
}
