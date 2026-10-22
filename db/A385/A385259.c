/*
 * A385259  Decimal expansion of the surface area of a gyroelongated square
 *          bicupola with unit edge.
 * https://oeis.org/A385259
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit (n-2) of the decimal expansion of 10+6*sqrt(3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 2   (so a(2) is the first term listed)                         */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BASE 1000000000u   /* 10^9 */

/* arbitrary precision integer (little‑endian, base 10^9) */
typedef struct {
    uint32_t *d;  /* digits, d[0] least significant word */
    size_t   len; /* number of used words */
    size_t   cap; /* allocated capacity */
} Big;

/* ------------------------------------------------------------ */
/* basic operations on Big                                     */
/* ------------------------------------------------------------ */

static void bi_reserve(Big *x, size_t need)
{
    if (need <= x->cap) return;
    size_t newcap = need * 2 + 10;
    uint32_t *p = (uint32_t *)realloc(x->d, newcap * sizeof(uint32_t));
    if (!p) exit(1);
    x->d = p;
    x->cap = newcap;
}

static void bi_init(Big *x)
{
    x->d   = NULL;
    x->len = 0;
    x->cap = 0;
}

static void bi_copy(Big *dst, const Big *src)
{
    bi_reserve(dst, src->len);
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

static int bi_cmp(const Big *a, const Big *b)
{
    if (a->len != b->len) return a->len > b->len ? 1 : -1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i]) return a->d[i] > b->d[i] ? 1 : -1;
    }
    return 0;
}

static void bi_mul_small_inplace(Big *x, uint32_t m)
{
    if (m == 0) { x->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t prod = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)(prod % BASE);
        carry   = prod / BASE;
    }
    if (carry) {
        bi_reserve(x, x->len + 1);
        x->d[x->len++] = (uint32_t)carry;
    }
}

static void bi_add_small_inplace(Big *x, uint32_t a)
{
    uint64_t carry = a;
    size_t i = 0;
    for ( ; i < x->len && carry; ++i) {
        uint64_t sum = (uint64_t)x->d[i] + carry;
        x->d[i] = (uint32_t)(sum % BASE);
        carry   = sum / BASE;
    }
    if (carry) {
        bi_reserve(x, x->len + 1);
        x->d[x->len++] = (uint32_t)carry;
    }
}

static void bi_sub_inplace(Big *a, const Big *b) /* assumes a >= b */
{
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t sub = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) - borrow;
        if (sub < 0) {
            sub += BASE;
            borrow = 1;
        } else
            borrow = 0;
        a->d[i] = (uint32_t)sub;
    }
    while (a->len && a->d[a->len - 1] == 0) --a->len;
}

/* ------------------------------------------------------------ */
/* main sequence routine                                        */
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* static state */
    static int initialized = 0;
    static Big rem, p, temp1, temp2, cand;
    static size_t next_group = 0;   /* next group index for sqrt digit */
    static size_t sqrt_len  = 0;    /* number of sqrt digits produced */
    static char *sqrt_digits = NULL;
    static size_t sqrt_cap  = 0;    /* capacity of sqrt_digits array */

    if (!initialized) {
        bi_init(&rem); bi_init(&p); bi_init(&temp1); bi_init(&temp2); bi_init(&cand);
        sqrt_digits = NULL;
        sqrt_cap    = 0;
        initialized = 1;
    }

    if (n == 2) return 2;
    if (n == 3) return 0;

    /* we need sqrt integer of length n-1 digits (sqrt digits)   */
    size_t need_len = n - 1;
    if (sqrt_len < need_len) {
        /* enlarge sqrt_digits array if needed */
        if (sqrt_cap < need_len) {
            sqrt_cap = need_len * 2 + 10;
            char *tmp = (char *)realloc(sqrt_digits, sqrt_cap);
            if (!tmp) exit(1);
            sqrt_digits = tmp;
        }

        /* extend sqrt digit generation */
        while (sqrt_len < need_len) {
            /* determine group value for this step */
            uint32_t group;
            if (next_group == 0)          group = 1;
            else if (next_group == 1)     group = 8;
            else                          group = 0;

            /* rem = rem*100 + group */
            bi_mul_small_inplace(&rem, 100);
            bi_add_small_inplace(&rem, group);

            /* temp1 = p*20          (p is current sqrt prefix) */
            bi_copy(&temp1, &p);
            bi_mul_small_inplace(&temp1, 20);

            /* find largest d (0..9) with (temp1 + d)*d <= rem */
            uint32_t d_val = 0;
            for (uint32_t d = 9; ; --d) {
                bi_copy(&temp2, &temp1);
                bi_add_small_inplace(&temp2, d);

                bi_copy(&cand, &temp2);
                bi_mul_small_inplace(&cand, d);

                if (bi_cmp(&cand, &rem) <= 0) {
                    d_val = d;
                    break;
                }
            }

            /* record digit */
            sqrt_digits[sqrt_len++] = (char)('0' + d_val);

            /* update next group index */
            ++next_group;

            /* p = p*10 + d_val  (new sqrt prefix) */
            bi_mul_small_inplace(&p, 10);
            bi_add_small_inplace(&p, d_val);

            /* rem = rem - cand  (new remainder) */
            bi_sub_inplace(&rem, &cand);
        }
    }

    /* after sqrt digits are ready:
       first two digits are the integer part '1''0' of sqrt(108)
       which correspond to the integer part '20' of the constant.
       The required output digit is:
          index 2 -> '2'
          index 3 -> '0'
          index n (n>=4) -> sqrt_digits[2 + (n-4)] '0'..'9'
    */
    size_t pos = n - 4;          /* 0‑based position in fractional part */
    return (seq_t)(sqrt_digits[2 + pos] - '0');
}
