/*
 * A293787  Numbers k such that (d(k), d(k+1)) = (0,0) in the base-2 digits
 *          d(i) of sqrt(3).
 * https://oeis.org/A293787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A293787: Numbers k such that (d(k), d(k+1)) = (0,0) in the binary digits
 * of sqrt(3).  This implementation generates the binary digits of sqrt(3)
 * digit by digit using a custom big‑integer type, stores the positions of
 * all consecutive zero pairs, and returns the n-th element.
 *
 * The code uses only integer arithmetic, standard headers and the
 * provided scalar types seq_t and idx_t. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ******** Helper big‑integer type ******** */
typedef struct {
    size_t len;        /* number of limbs (32‑bit words) */
    uint32_t *limb;    /* little‑endian limbs: limb[0] is least significant */
} BI;

/* Allocate a BI with capacity at least 'cap' limbs, zero‑initialized. */
static BI *bi_new(size_t cap)
{
    BI *x = (BI *)malloc(sizeof(BI));
    x->len  = cap > 0 ? cap : 1;
    x->limb = (uint32_t *)calloc(x->len, sizeof(uint32_t));
    return x;
}

/* Free a BI. */
static void bi_free(BI *x)
{
    free(x->limb);
    free(x);
}

/* Ensure BI has capacity for at least 'newlen' limbs. */
static void bi_grow(BI *x, size_t newlen)
{
    if (newlen > x->len) {
        x->limb = (uint32_t *)realloc(x->limb, newlen * sizeof(uint32_t));
        memset(x->limb + x->len, 0, (newlen - x->len) * sizeof(uint32_t));
        x->len = newlen;
    }
}

/* Remove leading zero limbs. */
static void bi_trim(BI *x)
{
    while (x->len > 1 && x->limb[x->len - 1] == 0)
        x->len--;
}

/* Set BI to an unsigned 32‑bit value. */
static void bi_set_uint32(BI *x, uint32_t v)
{
    bi_grow(x, 1);
    x->len   = 1;
    x->limb[0] = v;
}

/* Subtract an unsigned 32‑bit value (<= 0xffffffff) from BI. */
static void bi_sub_uint32(BI *x, uint32_t v)
{
    uint64_t borrow = 0;
    for (size_t i = 0; i < x->len; i++) {
        uint64_t a = x->limb[i];
        uint64_t sub = v + borrow;
        if (a < sub) {
            x->limb[i] = (uint32_t)(a + 0x100000000ull - sub);
            borrow = 1;
        } else {
            x->limb[i] = (uint32_t)(a - sub);
            borrow = 0;
        }
        v = 0;  /* only first limb uses v */
    }
    bi_trim(x);
}

/* Copy src into dest (dest is grown if necessary). */
static void bi_copy(BI *dest, const BI *src)
{
    if (dest->len < src->len)
        bi_grow(dest, src->len);
    memcpy(dest->limb, src->limb, src->len * sizeof(uint32_t));
    for (size_t i = src->len; i < dest->len; i++)
        dest->limb[i] = 0;
    dest->len = src->len;
    bi_trim(dest);
}

/* Compare two BIs: return -1 if a<b, 0 if a==b, 1 if a>b. */
static int bi_compare(const BI *a, const BI *b)
{
    if (a->len != b->len)
        return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i > 0; i--) {
        uint32_t av = a->limb[i - 1];
        uint32_t bv = b->limb[i - 1];
        if (av != bv)
            return (av > bv) ? 1 : -1;
    }
    return 0;
}

/* Subtract b from a (assumes a >= b). */
static void bi_sub(BI *a, const BI *b)
{
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; i++) {
        uint64_t ai = a->limb[i];
        uint64_t bi = (i < b->len) ? b->limb[i] : 0;
        uint64_t sub = bi + borrow;
        if (ai < sub) {
            a->limb[i] = (uint32_t)(ai + 0x100000000ull - sub);
            borrow = 1;
        } else {
            a->limb[i] = (uint32_t)(ai - sub);
            borrow = 0;
        }
    }
    bi_trim(a);
}

/* Shift BI left by 1 bit. */
static void bi_lshift1(BI *x)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; i++) {
        uint64_t tmp = ((uint64_t)x->limb[i] << 1) | carry;
        x->limb[i] = (uint32_t)tmp;
        carry = tmp >> 32;
    }
    if (carry) {
        x->limb = (uint32_t *)realloc(x->limb, (x->len + 1) * sizeof(uint32_t));
        x->limb[x->len] = (uint32_t)carry;
        x->len++;
    }
}

/* Shift BI left by 2 bits. */
static void bi_lshift2(BI *x)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; i++) {
        uint64_t tmp = ((uint64_t)x->limb[i] << 2) | carry;
        x->limb[i] = (uint32_t)tmp;
        carry = tmp >> 32;
    }
    if (carry) {
        x->limb = (uint32_t *)realloc(x->limb, (x->len + 1) * sizeof(uint32_t));
        x->limb[x->len] = (uint32_t)carry;
        x->len++;
    }
}

/* ******** Sequence computation ******** */
seq_t a(idx_t n)
{
    /* Persistent state: digit generator and found positions. */
    static BI *rem = NULL;          /* remainder */
    static BI *root = NULL;         /* partial root */
    static BI *cand = NULL;         /* candidate during comparisons */
    static bool init = false;
    static idx_t next_digit = 2;     /* index of next bit to be produced */
    static seq_t prev_bit = 0;       /* previous bit value */

    static seq_t *pos = NULL;       /* array of found positions k */
    static size_t pos_len = 0;      /* number of stored positions */
    static size_t pos_cap = 0;      /* allocated capacity for pos */

    if (n == 0) return 0;           /* undefined for n=0, return 0 */

    /* First time: initialise the big integers and first digit. */
    if (!init) {
        rem  = bi_new(1);
        root = bi_new(1);
        cand = bi_new(1);

        /* Process the integer part of sqrt(3): bring down '11' => rem = 3 */
        bi_set_uint32(rem, 3);
        /* subtract candidate 1 from rem */
        bi_sub_uint32(rem, 1);   /* rem becomes 2 */
        /* root = (root << 1) | 1 -> root becomes 1 */
        bi_lshift1(root);
        root->limb[0] |= 1;

        prev_bit = 1;            /* first digit (integer part) is 1 */
        next_digit = 2;          /* next bit index to be produced */
        init = true;
    }

    /* Generate digits until we have the desired number of positions. */
    while (pos_len < (size_t)n) {
        /* Bring down two zeros for fractional part. */
        bi_lshift2(rem);

        /* Compute candidate = (root << 2) | 1. */
        bi_copy(cand, root);
        bi_lshift2(cand);
        cand->limb[0] |= 1;
        bi_trim(cand);

        /* Decide the next bit. */
        seq_t cur_bit;
        if (bi_compare(rem, cand) >= 0) {
            /* Bit is 1. */
            bi_sub(rem, cand);
            bi_lshift1(root);
            root->limb[0] |= 1;
            cur_bit = 1;
        } else {
            /* Bit is 0. */
            bi_lshift1(root);
            cur_bit = 0;
        }

        /* Record a position when two consecutive zeros occur. */
        if (prev_bit == 0 && cur_bit == 0) {
            /* Position k is the index of the first zero. */
            seq_t k = (seq_t)(next_digit - 1);
            if (pos_len + 1 > pos_cap) {
                size_t new_cap = pos_cap ? pos_cap << 1 : 16;
                while (new_cap <= pos_len + 1) new_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(pos, new_cap * sizeof(seq_t));
                pos = tmp;
                pos_cap = new_cap;
            }
            pos[pos_len++] = k;
        }

        prev_bit = cur_bit;
        next_digit++;
    }

    return pos[n - 1];
}
