/*
 * A369961  a(n) = 1 if gcd(n, A003415(n)) is equal to gcd(n, A276086(n)),
 *          otherwise 0, where A003415 is the arithmetic derivative, and
 *          A276086 is the primorial base exp-function.
 * https://oeis.org/A369961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if gcd(n, A003415(n)) == gcd(n, A276086(n)), otherwise
 *            0 where A003415 is the arithmetic derivative and A276086 is the
 *            primorial base exp-function
 * Range:     u8 n <= 48, u16 n <= 86, u32 n <= 7897, u64 n <= 9951,
 *            u128 n <= 9951, big any n
 * Helpers:   factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

/* Some small primes – enough for all n that fit in the tests. */
static const uint64_t SMALL_PRIMES[20] = {2,3,5,7,11,13,17,19,23,29,
                                         31,37,41,43,47,53,59,61,67,71};

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0) {
        seq_t deriv = 0;
        seq_t prod  = 1;
        seq_t gn = gcd((seq_t)n, deriv);   /* gcd(0,0) = 0 */
        seq_t gp = gcd((seq_t)n, prod);    /* gcd(0,1) = 1 */
        return (gn == gp) ? 1 : 0;
    }

    /* ---------- arithmetic derivative ----------------- */
    /* Factor n */
    uint64_t pr[32];
    uint64_t ex[32];
    size_t  cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    seq_t deriv = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = ((seq_t)n / pr[i]) * ex[i];
        deriv += term;
    }

    /* ---------- primorial base exp-function ------------ */
    seq_t prod = 1;
    idx_t temp = n;
    size_t p_index = 0;

    while (temp > 0 && p_index < (sizeof(SMALL_PRIMES)/sizeof(SMALL_PRIMES[0]))) {
        uint64_t base  = SMALL_PRIMES[p_index];          /* radix for this digit */
        uint64_t digit = temp % base;                    /* current digit */
        temp /= base;                                    /* remaining value */

        /* multiply by prime^digit */
        prod *= pow_uint(SMALL_PRIMES[p_index], (idx_t)digit);
        ++p_index;
    }

    /* If we ran out of primes (very large n), the remaining
       digits would be zero – product unchanged, so we ignore it. */

    /* ---------- compare the gcds ----------------------- */
    seq_t gn = gcd((seq_t)n, deriv);
    seq_t gp = gcd((seq_t)n, prod);
    return (gn == gp) ? 1 : 0;
}
