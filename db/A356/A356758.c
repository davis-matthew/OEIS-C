/*
 * A356758  a(n) is the number of nonzero digits in n!.
 * https://oeis.org/A356758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Sequence A356758: a(n) = number of non‑zero decimal digits in n!   */
/*   offset 0, a(0) = 1 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0 || n == 1)
        return (seq_t)1;   /* 0! = 1! = 1 → one non‑zero digit */

    /* big integer representation in base 10^9 */
    const uint32_t BASE = 1000000000U;  /* < 2^32, fits in uint32_t */
    size_t len  = 1;                /* number of limbs used */
    size_t cap  = 32;               /* current allocation size */
    uint32_t *limbs = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!limbs) abort();            /* allocation failed */
    limbs[0] = 1;                   /* initial factorial value = 1 */

    /* multiply by 2 .. n */
    for (idx_t i = 2; i <= n; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint64_t prod   = (uint64_t)limbs[j] * (uint64_t)i + carry;
            limbs[j]        = (uint32_t)(prod % BASE);
            carry           = prod / BASE;
        }
        while (carry) {
            if (len == cap) {
                cap <<= 1;                               /* double capacity */
                uint32_t *tmp = (uint32_t *)realloc(limbs,
                                                     cap * sizeof(uint32_t));
                if (!tmp) abort();
                limbs = tmp;
            }
            limbs[len++] = (uint32_t)(carry % BASE);
            carry       /= BASE;
        }
    }

    /* count non‑zero decimal digits by formatting each limb */
    uint64_t cnt = 0;
    char buf[10];  /* 9 digits + NUL */
    for (size_t idx = len; idx > 0; --idx) {
        size_t i = idx - 1;          /* current limb index */
        int written;
        if (i == len - 1) {          /* most significant limb – no leading zeros */
            written = snprintf(buf, sizeof(buf), "%u", limbs[i]);
        } else {
            written = snprintf(buf, sizeof(buf), "%09u", limbs[i]);
        }
        for (int k = 0; k < written; ++k)
            if (buf[k] != '0')
                ++cnt;
    }

    free(limbs);
    return (seq_t)cnt;
}
