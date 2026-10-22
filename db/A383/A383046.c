/*
 * A383046  Smallest initial value for shortcut form of the Collatz function
 *          (3x+1)/2 sequence that begins with exactly n-1 increases and
 *          decreases once before the last increment.
 * https://oeis.org/A383046
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) * (3^(-n) mod 2^(floor(1+n*log2(3))+1-n)) - 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   bitlen_seq, mod_pow, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"
#include "../utils/bitlen_seq.h"
#include "../utils/mod_pow.h"

static seq_t inverse_mod_pow2(seq_t a, int k)
{
    seq_t x = (seq_t)1;          /* inverse modulo 2 */
    int bits = 1;
    while (bits < k) {
        int next_bits = bits << 1;
        seq_t mask = pow_seq((seq_t)2, next_bits) - (seq_t)1;
        seq_t prod = (a * x) & mask;
        seq_t t    = ((seq_t)2 - prod) & mask;
        x = (x * t) & mask;
        bits = next_bits;
    }
    seq_t mask = pow_seq((seq_t)2, k) - (seq_t)1;
    return x & mask;
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;          /* Undefined below offset */

    /* Compute y = bitlen(3^n) + 1 - n  ( = floor(1+n*log2(3)) + 1 - n ) */
    seq_t pow3n = pow_seq((seq_t)3, (int)n);
    unsigned bl = bitlen_seq(pow3n);
    int y = (int)bl + 1 - (int)n;
    if (y <= 0) return (seq_t)0;         /* Should not occur for n >= 3 */

    /* Modulus 2^y */
    seq_t mod = pow_seq((seq_t)2, y);

    /* 3^n modulo 2^y */
    seq_t pow3n_mod = mod_pow((seq_t)3, n, mod);

    /* Inverse of 3^n modulo 2^y */
    seq_t inv = inverse_mod_pow2(pow3n_mod, y);

    /* 2^(n-1) */
    seq_t pow2_n1 = pow_seq((seq_t)2, (int)(n - 1));

    /* Result: 2^(n-1) * inv - 1 */
    seq_t result = (pow2_n1 * inv) - (seq_t)1;

    return result;
}
