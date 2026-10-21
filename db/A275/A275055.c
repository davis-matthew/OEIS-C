/*
 * A275055  Irregular triangle read by rows listing divisors d of n in order
 *          of appearance in a matrix of products that arranges the powers of
 *          prime divisors p of n along independent axes.
 * https://oeis.org/A275055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1465, u16 n <= 7866, u32 n <= 7866, u64 n <= 7866,
 *            u128 n <= 7866, big any n
 * Helpers:   divisor_count_u64, factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) enumerates, row by row, the divisors of each integer
   n in the irregular order defined by the prime‑exponent grid: the exponent
   of the smallest prime varies fastest (inner loop).  The index n of the
   sequence is the position in the concatenated rows.  This function
   computes the n-th entry by finding the corresponding integer m and the
   position within its divisor list, then generating that divisor directly. */

#include <stdint.h>
#include <stddef.h>

/* Helpers provided by the framework */
#include "../utils/divisor_count_u64.h"
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"

/* Convert position (1‑based) to the corresponding divisor of m. */
static seq_t nth_divisor(idx_t m, idx_t pos)
{
    if (m == 1) return (seq_t)1;          /* 1 has only divisor 1 */

    /* Factor m */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)m, primes, exps, &cnt);

    /* Mixed‑radix conversion: the exponent of the smallest prime is the
       least significant digit (varies fastest). */
    idx_t idx = pos - 1;
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t base = (idx_t)(exps[i] + 1);   /* base for this digit */
        idx_t exp  = idx % base;
        idx      /= base;

        /* Multiply by p_i^exp.  pow_seq handles seq_t size (even arbitrary). */
        result = result * pow_seq((seq_t)primes[i], (int)exp);
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;          /* out of range behaviour is undefined */

    /* Find row m such that Σ_{k< m} τ(k) < n ≤ Σ_{k≤ m} τ(k) */
    idx_t cumulative = 0;
    idx_t m = 0;
    while (1) {
        idx_t tau_next = divisor_count_u64(m + 1);
        if (cumulative + tau_next >= n) break;
        cumulative += tau_next;
        ++m;
    }
    ++m;                     /* m is the integer whose row contains the n‑th term */
    idx_t pos = n - cumulative;   /* position inside row m (1‑based) */

    return nth_divisor(m, pos);
}
