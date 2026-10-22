/*
 * A304335  Sum of digits of (2*n-1)!!.
 * https://oeis.org/A304335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of digits of (2*n-1)!!
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Big‑integer in base 1,000,000,000 */
    enum { BASE = 1000000000u };
    size_t cap = 64;
    uint32_t *limbs = (uint32_t*)malloc(cap * sizeof(uint32_t));
    if (!limbs) return 0;          /* abort on allocation failure */
    limbs[0] = 1;                  /* start with 1 = 1!! */
    size_t len = 1;

    for (idx_t i = 1; i <= n; ++i) {
        uint32_t factor = (uint32_t)(2 * i - 1);   /* odd multiplier */
        uint64_t carry = 0;

        for (size_t j = 0; j < len; ++j) {
            uint64_t prod = (uint64_t)limbs[j] * factor + carry;
            limbs[j] = (uint32_t)(prod % BASE);
            carry   = prod / BASE;
        }
        while (carry) {
            if (len == cap) {
                cap <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(limbs, cap * sizeof(uint32_t));
                if (!tmp) { free(limbs); return 0; }
                limbs = tmp;
            }
            limbs[len++] = (uint32_t)carry;
            carry /= BASE;
        }
    }

    /* Sum decimal digits of the big integer */
    seq_t sum = 0;
    for (size_t j = 0; j < len; ++j) {
        uint32_t x = limbs[j];
        while (x) {
            sum += x % 10;
            x  /= 10;
        }
    }

    free(limbs);
    return sum;
}
