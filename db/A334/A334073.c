/*
 * A334073  Decimal expansion of Sum_{k >= 1} e(k)/2^k, where e(k) = 1 if
 *          gpf(k+1) > gpf(k) and 0 otherwise, and gpf(k) is the greatest
 *          prime dividing k (A006530).
 * https://oeis.org/A334073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the n-th decimal digit of * S = Σ_{k≥1} e(k)/2^k, *
 *            where e(k)=1 if gpf(k+1)>gpf(k) else 0. * a(n) = ⌊S·10^(n+1)⌋
 *            mod 10
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "stdbool.h"
#include "../utils/gpf.h"        /* uint64_t gpf(uint64_t) */

/* ------------------------------------------------------------------ */
/* Small big‑integer routines (base 2^32) – only what we need.        */
/* ------------------------------------------------------------------ */
typedef struct {
    uint32_t *w;      /* little‑endian words */
    size_t   len;     /* words in use (>=1) */
} bi_t;

static void bi_init(bi_t *a, size_t len)
{
    a->w   = (uint32_t*)calloc(len, sizeof(uint32_t));
    a->len = (len>0)?len:1;
}

static void bi_free(bi_t *a)
{
    free(a->w);
    a->w   = NULL;
    a->len = 0;
}

/* shift left 1 bit, then OR e (0 or 1) into LSB */
static void bi_shift_left_one_and_add(bi_t *a, uint32_t e)
{
    uint32_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t t = ((uint64_t)a->w[i] << 1) | carry;
        a->w[i]    = (uint32_t)t;
        carry      = (uint32_t)(t >> 32);
    }
    if (carry) {
        a->w[a->len] = carry;
        a->len++;
    }
    a->w[0] |= e;                 /* set low bit */
}

/* multiply entire integer by small m (m <= 10 in this program) */
static void bi_mul_small(bi_t *a, uint32_t m)
{
    uint32_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t t = (uint64_t)a->w[i] * m + carry;
        a->w[i]    = (uint32_t)t;
        carry      = (uint32_t)(t >> 32);
    }
    if (carry) {
        a->w[a->len] = carry;
        a->len++;
    }
}

/* keep only the low K bits: r = r mod 2^K */
static void bi_mask_low_k(bi_t *r, size_t K)
{
    size_t ws = K >> 5;  /* /32 */
    size_t bs = K & 31;  /* %32 */

    if (bs == 0) {               /* discard word ws and higher */
        if (ws < r->len) r->len = ws;
    } else {
        if (ws >= r->len) {
            /* nothing to keep beyond lower 32*ws bits */
            r->len = ws;
        } else {
            r->w[ws] &= ((uint32_t)1 << bs) - 1;
            /* discard higher words */
            for (size_t i = ws + 1; i < r->len; ++i) r->w[i] = 0;
            r->len = ws + 1;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Choose K such that 2^K > 2·10^(n+1), i.e. K > (n+1)·log2(10)+1.
       Using K = 4·(n+1)+2 is safe for all n≥0. */
    const size_t K = 4 * ((size_t)n + 1) + 2;

    /* Number of 32‑bit words needed for numbers up to K+4 bits. */
    const size_t WORDS = (K + 32) / 32 + 2;

    /* Build the binary numerator N = Σ e(k)·2^(K-k)   (K bits). */
    bi_t N;
    bi_init(&N, WORDS);
    for (size_t k = 1; k <= K; ++k) {
        uint64_t gk  = gpf(k);
        uint64_t gk1 = gpf(k + 1);
        uint32_t e  = (gk1 > gk) ? 1U : 0U;
        bi_shift_left_one_and_add(&N, e);
    }

    /* Prepare remainder r = N (will be updated during digit extraction). */
    bi_t r;
    bi_init(&r, WORDS);
    for (size_t i = 0; i < N.len; ++i) r.w[i] = N.w[i];
    r.len = N.len;

    /* Extract digits one by one. */
    for (size_t i = 0; i <= (size_t)n; ++i) {
        bi_mul_small(&r, 10);               /* r = r * 10          */

        /* quotient = r >> K   (value < 10)  */
        size_t ws = K >> 5;   /* word index */
        size_t bs = K & 31;   /* bit offset */
        uint32_t low_word  = (ws < r.len) ? r.w[ws] : 0;
        uint32_t high_word = (ws + 1 < r.len) ? r.w[ws + 1] : 0;
        uint32_t digit;
        if (bs == 0) {
            digit = low_word;
        } else {
            digit = (low_word >> bs) | (high_word << (32 - bs));
        }

        /* keep only the low K bits as remainder for next step */
        bi_mask_low_k(&r, K);

        if (i == (size_t)n) {          /* last iteration – we have the answer */
            bi_free(&N);
            bi_free(&r);
            return (seq_t)digit;
        }
    }

    /* Should never reach here */
    bi_free(&N);
    bi_free(&r);
    return 0;
}
