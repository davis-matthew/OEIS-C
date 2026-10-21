/*
 * A253638  Number of zeros in the decimal expansion of 5^n.
 * https://oeis.org/A253638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of zeros in the decimal expansion of 5^n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Arbitrary-precision integer in base 1e9, single-precision multiply */
struct BigInt {
    uint32_t *digits;    /* least significant block first */
    size_t   len;        /* number of used blocks */
    size_t   cap;        /* allocated capacity */
};

static const uint32_t BIGBASE = 1000000000U;  /* 1e9 */

static struct BigInt *bigint_new(void)
{
    struct BigInt *b = (struct BigInt *)malloc(sizeof(struct BigInt));
    if (!b) return NULL;
    b->cap   = 4;
    b->digits= (uint32_t *)calloc(b->cap, sizeof(uint32_t));
    if (!b->digits) { free(b); return NULL; }
    b->len = 1;
    b->digits[0] = 1;   /* start with value 1 */
    return b;
}

static void bigint_grow(struct BigInt *b)
{
    if (b->len < b->cap) return;
    size_t new_cap = b->cap * 2;
    uint32_t *new_digits = (uint32_t *)realloc(b->digits, new_cap * sizeof(uint32_t));
    if (new_digits) {
        b->digits = new_digits;
        b->cap    = new_cap;
        for (size_t i = b->len; i < new_cap; ++i) b->digits[i] = 0;
    }
}

static void bigint_mul_small(struct BigInt *b, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->digits[i] * m + carry;
        b->digits[i] = (uint32_t)(prod % BIGBASE);
        carry = prod / BIGBASE;
    }
    while (carry > 0) {
        bigint_grow(b);
        b->digits[b->len++] = (uint32_t)(carry % BIGBASE);
        carry /= BIGBASE;
    }
}

static void bigint_free(struct BigInt *b)
{
    if (b) {
        free(b->digits);
        free(b);
    }
}

/* Count zeros in decimal digits of the big integer */
static seq_t count_decimal_zeros(const struct BigInt *b)
{
    seq_t sum = 0;
    for (size_t idx = b->len; idx-- > 0; ) {
        uint32_t v = b->digits[idx];
        if (idx == b->len - 1) {            /* most significant block */
            if (v == 0) {                   /* 5^0 case */
                /* representation is "1", no zeros */
            } else {
                while (v > 0) {
                    if ((v % 10) == 0) sum++;
                    v /= 10;
                }
            }
        } else {                            /* other blocks: must write 9 digits with leading zeros */
            for (int k = 0; k < 9; ++k) {
                if (v % 10 == 0) sum++;
                v /= 10;
            }
        }
    }
    return sum;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Special case: 5^0 = 1, zero zeros */
    if (n == 0) return 0;

    struct BigInt *b = bigint_new();
    if (!b) return 0; /* allocation failed */

    for (idx_t i = 1; i <= n; ++i) {
        bigint_mul_small(b, 5);
    }

    seq_t result = count_decimal_zeros(b);
    bigint_free(b);
    return result;
}
