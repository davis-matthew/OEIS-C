/*
 * A278081  a(n) is 1/12 of the number of primitive quadruples with sum = 0
 *          and sum of squares = 2*m^2, where m = 2*n - 1.
 * https://oeis.org/A278081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 108, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = b(2*n-1), where  
 *   b(m) is multiplicative, b(even)=0,  
 *   b(p^k)=p^(k-1)*b(p) for an odd prime p,k≥1,  
 *   b(p)= 2          if p=3,  
 *   b(p)= p+1        if p≡5,7,13,23 (mod 24),  
 *   b(p)= p-1        if p≡1,11,17,19 (mod 24).  
 *   b(1)=1.  
 * The function factorises m=2*n-1 and applies these rules.  
 */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* m = 2*n - 1 is odd for n ≥ 1 */
    idx_t m = 2ULL * n - 1ULL;
    if (m <= 1ULL)          /* m = 1 => a(1) = 1 */
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t mm = m;

    /* trial division factorisation */
    for (idx_t p = 3ULL; p <= mm / p; p += 2ULL) {
        if (mm % p != 0ULL)
            continue;

        /* count multiplicity of prime p */
        size_t exp = 0;
        while (mm % p == 0ULL) {
            mm /= p;
            ++exp;
        }

        /* compute p^(exp-1) */
        seq_t p_pow = (seq_t)1;
        for (size_t i = 1; i < exp; ++i)
            p_pow *= (seq_t)p;

        /* base value b(p) */
        seq_t bp;
        if (p == 3ULL) {
            bp = (seq_t)2;          /* b(3) = 2 */
        } else {
            idx_t mod = p % 24ULL;
            if (mod == 5ULL || mod == 7ULL || mod == 13ULL || mod == 23ULL)
                bp = (seq_t)p + (seq_t)1;
            else if (mod == 1ULL || mod == 11ULL || mod == 17ULL || mod == 19ULL)
                bp = (seq_t)p - (seq_t)1;
            else
                bp = (seq_t)0;        /* should not happen for odd primes */
        }

        result *= p_pow * bp;
    }

    /* leftover prime factor (if any) */
    if (mm > 1ULL) {
        idx_t p = mm;
        seq_t bp;
        if (p == 3ULL) {
            bp = (seq_t)2;
        } else {
            idx_t mod = p % 24ULL;
            if (mod == 5ULL || mod == 7ULL || mod == 13ULL || mod == 23ULL)
                bp = (seq_t)p + (seq_t)1;
            else if (mod == 1ULL || mod == 11ULL || mod == 17ULL || mod == 19ULL)
                bp = (seq_t)p - (seq_t)1;
            else
                bp = (seq_t)0;
        }
        result *= bp;          /* exp = 1 -> p^(exponent-1) = 1 */
    }

    return result;
}
