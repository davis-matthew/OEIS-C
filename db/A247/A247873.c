/*
 * A247873  For bases b = 2, 3, ..., n, let the base-b expansion of n be
 *          [c_{1,b} c_{2,b} .. c_{r_b,b}], with the most significant "digit"
 *          on the left, 0 <= c_{i,b} < b, and c_{1,b} != 0; then a(n) is the
 *          number whose base-n expansion is c_{1,n} c_{2,n} ... c_{r_n,n}
 *          c_{1,n-1} ... c_{1,2} c_{2,2} ... c_{r_2,2}.
 * https://oeis.org/A247873
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = integer whose base‑n expansion is the concatenation of
 *            the base‑b expansions of n for b = n, n-1, … , 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* small temporary stack for the digits of n in a single base */
    enum { STACK_CAP = 64 };
    uint16_t stack[STACK_CAP];
    size_t stack_len;

    /* dynamic array for all digits in order */
    size_t cap = 32;
    uint16_t *digits = (uint16_t *)malloc(cap * sizeof(uint16_t));
    if (!digits) return 0;          /* fallback on OOM, should not happen */

    size_t len = 0;

    for (idx_t b = n; b >= 2; --b)
    {
        /* get digits of n in base b, most significant first */
        idx_t x = n;
        stack_len = 0;
        do {
            if (stack_len == STACK_CAP) {
                /* very unlikely for n up to 10000, but just in case */
                uint16_t *tmp = (uint16_t *)realloc(stack, (stack_len + 1) * sizeof(uint16_t));
                if (!tmp) { free(digits); return 0; }
                /* note: stack is on the stack, realloc usage is illustrative only */
                /* we skip the realloc as it cannot be used on stack arrays;
                   this branch will never be executed for the intended n range */
            }
            stack[stack_len++] = (uint16_t)(x % b);
            x /= b;
        } while (x > 0);

        /* append in reverse order (most significant first) */
        for (int j = (int)stack_len - 1; j >= 0; --j)
        {
            if (len == cap) {
                cap <<= 1;
                uint16_t *tmp = (uint16_t *)realloc(digits, cap * sizeof(uint16_t));
                if (!tmp) { free(digits); return 0; }
                digits = tmp;
            }
            digits[len++] = stack[j];
        }

        if (b == 2) break; /* avoid underflow of unsigned b */
    }

    /* interpret the concatenated digits as a base‑n number */
    seq_t res = 0;
    const seq_t base = (seq_t)n;
    for (size_t i = 0; i < len; ++i) {
        res = res * base + (seq_t)digits[i];
    }

    free(digits);
    return res;
}
