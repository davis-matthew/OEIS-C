/*
 * A391992  Number of digits in the decimal expansion of binomial(2*n,n).
 * https://oeis.org/A391992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of decimal digits of binomial(2*n,n)
 * Range:     u8 n <= 426, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* number of digits of an unsigned 32‑bit value */
static inline int digits_uint32(uint32_t v)
{
    if (v >= 1000000000U) return 10;
    if (v >= 100000000U)  return 9;
    if (v >= 10000000U)   return 8;
    if (v >= 1000000U)    return 7;
    if (v >= 100000U)     return 6;
    if (v >= 10000U)      return 5;
    if (v >= 1000U)       return 4;
    if (v >= 100U)        return 3;
    if (v >= 10U)         return 2;
    return 1;
}

/* small integer gcd, 32 bit */
static inline uint32_t gcd_uint32(uint32_t a, uint32_t b)
{
    while (b) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* multiply big integer (base 1e9) by a small unsigned 32‑bit integer */
static void mul_small(uint32_t *big, size_t *len, uint32_t mul, size_t *cap)
{
    const uint32_t BASE = 1000000000U;
    uint64_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        uint64_t prod = (uint64_t)big[i] * mul + carry;
        big[i] = (uint32_t)(prod % BASE);
        carry  = prod / BASE;
    }
    while (carry) {
        if (*len >= *cap) {
            size_t new_cap = *cap * 2;
            uint32_t *tmp = (uint32_t *)realloc(big, new_cap * sizeof(uint32_t));
            if (!tmp) exit(EXIT_FAILURE);
            big = tmp;
            *cap = new_cap;
        }
        big[(*len)++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* divide big integer (base 1e9) by a small unsigned 32‑bit integer (exact division) */
static void div_small(uint32_t *big, size_t *len, uint32_t divisor)
{
    const uint32_t BASE = 1000000000U;
    uint64_t rem = 0;
    for (size_t i = *len; i > 0; --i) {
        uint64_t cur = big[i-1] + rem * BASE;
        big[i-1] = (uint32_t)(cur / divisor);
        rem      = cur % divisor;
    }
    while (*len > 1 && big[*len-1] == 0) (*len)--;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;               /* binomial(0,0)=1 */

    /* big integer storage: base 1e9 */
    size_t cap = 64;
    uint32_t *big = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!big) exit(EXIT_FAILURE);
    big[0] = 1;
    size_t len = 1;

    /* iterative computation of binomial(2n,n) */
    for (idx_t i = 1; i <= n; ++i) {
        uint32_t top   = (uint32_t)(n + i);        /* n+i  */
        uint32_t bottom = (uint32_t)(i);           /* i    */

        /* reduce the fraction top/bottom */
        uint32_t g = gcd_uint32(top, bottom);
        top   /= g;
        bottom /= g;

        /* multiply by the (reduced) top */
        mul_small(big, &len, top, &cap);

        /* divide by the (reduced) bottom – exact division */
        div_small(big, &len, bottom);
    }

    /* count decimal digits of the resulting big integer */
    uint32_t high = big[len-1];
    int high_digits = digits_uint32(high);
    seq_t result = (seq_t)((len-1) * 9 + high_digits);

    free(big);
    return result;
}
