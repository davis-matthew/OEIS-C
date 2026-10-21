/*
 * A271747  Number of set partitions of [n] such that 8 is the largest element
 *          of the last block.
 * https://oeis.org/A271747
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64 + 91*2^(n-6) + 245*2^(2*n-15) + 11*2^(n-7)*3^(n-8) +
 *            217*3^(n-7) + 161*5^(n-8) + 7^(n-8) for n > 8 a(8) = 1754
 * Range:     u8 none, u16 n <= 10, u32 n <= 18, u64 n <= 30, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// Use the provided big_pow over the std::bigint (or built‑in) type seq_t.
#include <stdint.h>
#include <stdbool.h>

/* Compute base^exp using exponentiation by squaring.
   Works for all supported seq_t types (built‑in or arbitrary precision).*/
static inline seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n < 8) return 0;           /* undefined below the offset */
    if (n == 8) return (seq_t)1754; /* special case */

    /* Precompute the needed powers */
    const seq_t two   =  (seq_t)2;
    const seq_t three =  (seq_t)3;
    const seq_t five  =  (seq_t)5;
    const seq_t seven =  (seq_t)7;

    seq_t pow2_n6   = pow_seq(two,   n - 6);          /* 2^(n-6)            */
    seq_t pow2_2n15 = pow_seq(two, 2*n - 15);          /* 2^(2*n-15)          */
    seq_t pow2_n7   = pow_seq(two,   n - 7);          /* 2^(n-7)             */
    seq_t pow3_n8   = pow_seq(three, n - 8);          /* 3^(n-8)             */
    seq_t pow3_n7   = pow_seq(three, n - 7);          /* 3^(n-7)             */
    seq_t pow5_n8   = pow_seq(five,  n - 8);          /* 5^(n-8)             */
    seq_t pow7_n8   = pow_seq(seven, n - 8);          /* 7^(n-8)             */

    /* 11 * 2^(n-7) * 3^(n-8) */
    seq_t part_11 = (seq_t)11 * (pow2_n7 * pow3_n8);

    /* Accumulate the result */
    seq_t val = (seq_t)64;
    val += (seq_t)91   * pow2_n6;
    val += (seq_t)245  * pow2_2n15;
    val += part_11;
    val += (seq_t)217  * pow3_n7;
    val += (seq_t)161  * pow5_n8;
    val += pow7_n8;

    return val;
}
