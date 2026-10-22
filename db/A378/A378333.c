/*
 * A378333  Decimal expansion of the base 9 Champernowne constant.
 * https://oeis.org/A378333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = decimal expansion of base‑9 Champernowne constant
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   next_base9_digit_slow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the first (n+1) decimal digits by
   building the rational value of the first M base‑9 digits,
   scaling by 10^(n+1), and dividing by 9^M using an arbitrary‑precision */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/next_base9_digit_slow.h"

/*  ---- Big integer (base 1e9) for intermediate calculations  ----  */
typedef struct {
    size_t len;    /* number of used limbs (least‑significant first) */
    size_t cap;    /* allocated limbs */
    uint32_t *d;   /* limbs, each < BASE */
} BigInt;

#define BASE 1000000000U   /* 1e9 */

/* initialise to zero */
static inline void bigint_init(BigInt *b)
{
    b->cap = 8;
    b->len = 0;
    b->d  = (uint32_t*)malloc(b->cap * sizeof(uint32_t));
}

/* free memory */
static inline void bigint_free(BigInt *b)
{
    free(b->d);
}

/* ensure capacity for n limbs */
static inline void bigint_grow(BigInt *b, size_t n)
{
    if (b->cap < n) {
        while (b->cap < n) b->cap <<= 1;
        b->d = (uint32_t*)realloc(b->d, b->cap * sizeof(uint32_t));
    }
}

/* multiply by small integer m (m <= 10) */
static inline void bigint_mul_small(BigInt *b, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)(prod % BASE);
        carry   = prod / BASE;
    }
    if (carry) {
        bigint_grow(b, b->len + 1);
        b->d[b->len++] = (uint32_t)carry;
    }
}

/* add small integer a (a <= 8) */
static inline void bigint_add_small(BigInt *b, uint32_t a)
{
    uint64_t carry = a;
    for (size_t i = 0; i < b->len && carry; ++i) {
        uint64_t sum = (uint64_t)b->d[i] + carry;
        b->d[i] = (uint32_t)(sum % BASE);
        carry   = sum / BASE;
    }
    if (carry) {
        bigint_grow(b, b->len + 1);
        b->d[b->len++] = (uint32_t)carry;
    }
}

/* divide by small integer d (d <= 10).  Returns remainder.  */
static inline void bigint_div_small(BigInt *b, uint32_t d, uint32_t *rem)
{
    uint64_t r = 0;
    for (size_t i = b->len; i-- > 0;) {
        uint64_t cur = r * BASE + b->d[i];
        b->d[i] = (uint32_t)(cur / d);
        r       = cur % d;
    }
    while (b->len && b->d[b->len - 1] == 0) --b->len;
    if (rem) *rem = (uint32_t)r;
}

/* ---- build integer value of first M base‑9 digits ----  */
static void big_generate_prefix(BigInt *res, idx_t M)
{
    bigint_init(res);
    uint64_t next_num = 1;
    size_t len = 0, pos = 0;
    uint8_t buf[32];
    idx_t count = 0;
    while (count < M) {
        uint8_t d = next_base9_digit_slow(&next_num, buf, &len, &pos);
        bigint_mul_small(res, 9);
        bigint_add_small(res, d);
        ++count;
    }
}

/* ---- cache for digits already computed ----  */
static seq_t   *cached_digits = NULL;
static size_t   cached_len    = 0;   /* number of digits stored */

/* ---- compute decimal digits up to requested n (0‑based) ----  */
static void compute_up_to(idx_t n)
{
    /* maximum needed decimal digits */
    size_t need = (size_t)n + 1;

    /* new cache size */
    size_t new_cap = need;
    if (cached_digits) {
        if (new_cap <= cached_len) return;   /* already done */
        cached_digits = (seq_t*)realloc(cached_digits, new_cap * sizeof(seq_t));
    } else {
        cached_digits = (seq_t*)malloc(new_cap * sizeof(seq_t));
    }
    cached_len = new_cap;

    /* ----- determine number of base‑9 digits needed ----- */
    /* find smallest M with 9^M > 8 * 10^(n+1)  */
    /* use double arithmetic for the estimate */
    const double LOG10_9  = 0.9542425094393249;   /* log10(9)          */
    const double LOG10_8  = 0.903089987;          /* log10(8)          */
    double mlog10 = ((double)n + 1.0 + LOG10_8) / LOG10_9;
    idx_t M = (idx_t)ceil(mlog10) + 2;           /* +2 gives margin   */

    /* ----- build big integer of first M base‑9 digits ----- */
    BigInt val;
    big_generate_prefix(&val, M);

    /* ----- multiply by 10^(n+1) ----- */
    for (idx_t i = 0; i < (idx_t)need; ++i)
        bigint_mul_small(&val, 10);

    /* ----- divide by 9^M (M successive divisions by 9) ----- */
    for (idx_t i = 0; i < M; ++i)
        bigint_div_small(&val, 9, NULL);

    /* ----- extract at most n+1 decimal digits from 'val' ----- */
    for (idx_t i = n; ; --i) {
        uint32_t rem;
        bigint_div_small(&val, 10, &rem);
        cached_digits[i] = (seq_t)rem;
        if (i == 0) break;
    }

    bigint_free(&val);
}

/* ---------------------------------------------------------------------- */
/*  Public sequence function                                              */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n >= cached_len) compute_up_to(n);
    return cached_digits[n];
}
