/*
 * A385135  The sum of divisors d of n such that n/d is an exponentially odd
 *          number (A268335).
 * https://oeis.org/A385135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e dividing n of * a(p^e)
 *            where a(p^e) = p^e + (p^(e+1)-1)/(p^2-1) if e is odd, * and
 *            a(p^e) = p^e + (p^(e+1)-p)/(p^2-1) if e is even
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Computes integer power for seq_t values (base^exp). */
static seq_t pow_seq_local(seq_t base, unsigned int exp) {
    seq_t result = 1;
    for (unsigned int i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for 0, return 0 */
    seq_t res = 1;
    uint64_t temp = (uint64_t)n;

    /* factorization by trial division */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            unsigned int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            seq_t p_seq = (seq_t)p;
            seq_t pe = pow_seq_local(p_seq, e);           /* p^e  */
            seq_t pe_next = pe * p_seq;                   /* p^(e+1) */
            seq_t denom = p_seq * p_seq - 1;              /* p^2 - 1 */

            seq_t numer;
            if (e % 2 == 0) {                            /* e even */
                numer = pe_next - p_seq;                 /* p^(e+1) - p */
            } else {                                     /* e odd  */
                numer = pe_next - 1;                     /* p^(e+1) - 1 */
            }

            seq_t term2 = numer / denom;                 /* integer division */
            seq_t term = pe + term2;                     /* a(p^e) */
            res *= term;
        }
    }

    /* remaining prime factor > 1 (if any) */
    if (temp > 1) {
        uint64_t p = temp;
        unsigned int e = 1;   /* exponent is 1 */
        seq_t p_seq = (seq_t)p;
        seq_t pe = p_seq;                              /* p^1 */
        seq_t pe_next = pe * p_seq;                    /* p^2 */
        seq_t denom = p_seq * p_seq - 1;               /* p^2 - 1 */
        seq_t numer = pe_next - 1;                     /* e is odd (1) so use -1 */
        seq_t term2 = numer / denom;
        seq_t term = pe + term2;
        res *= term;
    }

    return res;
}
