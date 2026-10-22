/*
 * A389231  Number of decimal digits in the n-th Catalan number.
 * https://oeis.org/A389231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of decimal digits of the n‑th Catalan number,
 *            computed directly as a(0)=1 and a(n) = a(n-1) for odd n, a(n) =
 *            floor(log10(Catalan(n)))+1 for even n via a multiplicative
 *            algorithm suitable for all index values
 * Range:     u8 n <= 430, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define BASE 1000000000U   /* 10^9 */

typedef struct {
    uint32_t *limb;  /* least significant limb first */
    size_t    len;   /* number of used limbs */
} bigInt;

/* initialise bigInt with value 1, limb array allocated with max_len capacity */
static void bigint_init(bigInt *a, size_t max_len)
{
    a->len = 1;
    a->limb = (uint32_t *)calloc(max_len, sizeof(uint32_t));
    a->limb[0] = 1U;
}

/* multiply bigInt a by small integer m */
static void bigint_mul_small(bigInt *a, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->limb[i] * m + carry;
        a->limb[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    while (carry) {
        a->limb[a->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* divide bigInt a by small integer d (exact division) */
static void bigint_div_small(bigInt *a, uint32_t d)
{
    uint64_t rem = 0;
    size_t i = a->len;
    while (i--) {
        uint64_t cur = a->limb[i] + rem * BASE;
        a->limb[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    /* trim leading zero limbs */
    while (a->len > 1 && a->limb[a->len - 1] == 0U)
        --a->len;
}

/* count decimal digits of bigInt a (a ≥ 1) */
static seq_t bigint_digits(const bigInt *a)
{
    /* digits of most significant limb */
    uint32_t ms = a->limb[a->len - 1];
    int ms_digits = 0;
    do { ++ms_digits; ms /= 10; } while (ms);
    /* remaining limbs contribute full 9 digits each */
    return (seq_t)(ms_digits + 9 * (a->len - 1));
}

/* compute a(n) = number of decimal digits of Catalan_n */
seq_t a(idx_t n)
{
    /* Catalan(0) = 1 => 1 digit */
    if (n == 0) return 1U;

    /* allocate big integer with sufficient limbs:
       final number of digits ≈ n * log10(4)  < 0.62n.
       9 digits per limb → upper bound on limbs ≈ 0.062n + 3.  */
    size_t max_len = (size_t)(n / 15 + 10);
    bigInt x;
    bigint_init(&x, max_len);

    /* compute binomial(2n, n) = ∏_{i=1}^n (n+i)/i  */
    for (uint64_t i = 1; i <= (uint64_t)n; ++i) {
        bigint_mul_small(&x, (uint32_t)(n + i));
        bigint_div_small(&x, (uint32_t)i);
    }

    /* divide by (n+1) to get Catalan(n) */
    bigint_div_small(&x, (uint32_t)(n + 1));

    seq_t digits = bigint_digits(&x);
    free(x.limb);
    return digits;
}
