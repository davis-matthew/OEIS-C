/*
 * A261628  Least prime p such that n-(p*n'-1) and n+(p*n'-1) are both prime
 *          where n' = (3+(-1)^n)/2, or 0 if no such prime p exists.
 * https://oeis.org/A261628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10000, u32 n <= 3, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 8509 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = least prime p such that 
          n - (p * n' - 1)   and   n + (p * n' - 1) 
   are both prime, where n' = (3 + (-1)^n)/2; 
   return 0 if no such prime exists. 
   For small n this yields the values from the OEIS test set.
*/
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1; guard against accidental 0 */
        return 0;

    seq_t n_val = (seq_t)n;
    /* n' = 2 if n even, else 1 */
    seq_t nprime = ( (n & 1ULL) ? 1 : 2 );

    /* upper bound for p from the conjecture: sqrt(2n) * log(5n) */
    double ub = sqrt(2.0 * (double)n) * log(5.0 * (double)n);
    uint64_t p_limit = (uint64_t)ceil(ub);
    if (p_limit < 2) p_limit = 2;
    seq_t p_limit_seq = (seq_t)p_limit + 1;   /* make bound inclusive */

    for (seq_t p = 2; p < p_limit_seq; ++p) {
        if (!is_prime_seq((uint64_t)p))
            continue;

        /* diff = p * n' - 1 */
        seq_t diff = p * nprime - 1;
        seq_t a_val = n_val - diff;
        seq_t b_val = n_val + diff;

        if (a_val < 2 || b_val < 2)
            continue;

        if (is_prime_big(a_val) && is_prime_big(b_val))
            return (seq_t)p;   /* found the least prime */
    }

    /* No prime found within the bound – return 0. */
    return 0;
}
