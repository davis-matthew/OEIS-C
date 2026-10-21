/*
 * A247454  Numbers k such that d(r,k) = d(s,k), where d(x,k) = k-th binary
 *          digit of x, r = {sqrt(2)}, s = {sqrt(3)}, and { } = fractional
 *          part.
 * https://oeis.org/A247454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Wrong formula: a(n) is NOT n*(n+1)/2.
/* Sequence: A247454 – numbers k such that the k‑th binary
 * digit of {sqrt(2)} equals that of {sqrt(3)}.
 *
 * The implementation computes successive bits of sqrt(2) and sqrt(3)
 * by the classical digit‑by‑digit binary extraction algorithm.
 * Values are cached in a static array so that repeated calls
 * are O(1) after the first generation. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- minimal big‑integer type (base 2^32) ---------- */
typedef struct {
    uint32_t *limb;   /* least significant limb first */
    size_t len;       /* number of used limbs (no leading zeros) */
    size_t cap;       /* allocated capacity in limbs */
} bi_t;

static void bi_init(bi_t *a) { a->limb = NULL; a->len = a->cap = 0; }
static void bi_free(bi_t *a) { free(a->limb); a->len = a->cap = 0; }

/* ensure at least 'need' limbs of storage */
static void bi_ensure(bi_t *a, size_t need) {
    if (need <= a->cap) return;
    size_t newcap = a->cap ? a->cap << 1 : 16;
    while (newcap < need) newcap <<= 1;
    void *tmp = realloc(a->limb, newcap * sizeof(uint32_t));
    if (!tmp) abort();
    a->limb = (uint32_t *)tmp;
    a->cap = newcap;
}

/* set to unsigned 32‑bit integer 'v' */
static void bi_set_uint32(bi_t *a, uint32_t v) {
    bi_ensure(a, 1);
    a->limb[0] = v;
    a->len = 1;
}

/* copy a to b */
static void bi_copy(bi_t *b, const bi_t *a) {
    bi_ensure(b, a->len);
    memcpy(b->limb, a->limb, a->len * sizeof(uint32_t));
    b->len = a->len;
}

/* shift left by 'bits' (1 or 2) */
static void bi_shift_left(bi_t *a, unsigned bits) {
    if (bits == 0) return;
    size_t oldlen = a->len;
    size_t newlen = oldlen + ((bits + 31) >> 5);
    bi_ensure(a, newlen);
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < oldlen; ++i) {
        uint64_t v = ((uint64_t)a->limb[i] << bits) | carry;
        a->limb[i] = (uint32_t)(v & 0xFFFFFFFFU);
        carry = v >> 32;
    }
    if (carry) {
        a->limb[oldlen] = (uint32_t)carry;
        a->len = oldlen + 1;
    } else {
        a->len = oldlen;
    }
    /* remove any high zero limbs that might appear after shift */
    while (a->len > 0 && a->limb[a->len - 1] == 0) a->len--;
}

/* add small unsigned 32‑bit value to big int */
static void bi_add_small(bi_t *a, uint32_t v) {
    uint64_t carry = v;
    size_t i = 0;
    while (carry && i < a->len) {
        uint64_t sum = (uint64_t)a->limb[i] + carry;
        a->limb[i] = (uint32_t)(sum & 0xFFFFFFFFU);
        carry = sum >> 32;
        ++i;
    }
    if (carry) {
        bi_ensure(a, a->len + 1);
        a->limb[a->len] = (uint32_t)carry;
        a->len++;
    }
}

/* compare a and b:  >0 if a>b, 0 if equal, <0 if a<b */
static int bi_cmp(const bi_t *a, const bi_t *b) {
    if (a->len != b->len) return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i > 0; --i) {
        uint32_t av = a->limb[i - 1];
        uint32_t bv = b->limb[i - 1];
        if (av != bv) return (av > bv) ? 1 : -1;
    }
    return 0;
}

/* subtract b from a (a >= b).  Assumes a >= b. */
static void bi_sub_assign(bi_t *a, const bi_t *b) {
    uint64_t borrow = 0;
    size_t i;
    for (i = 0; i < b->len; ++i) {
        uint64_t ai = a->limb[i];
        uint64_t bi = b->limb[i];
        uint64_t sub = ai - bi - borrow;
        a->limb[i] = (uint32_t)(sub & 0xFFFFFFFFU);
        borrow = (ai < bi + borrow) ? 1 : 0;
    }
    for (; i < a->len && borrow; ++i) {
        uint64_t ai = a->limb[i];
        uint64_t sub = ai - borrow;
        a->limb[i] = (uint32_t)(sub & 0xFFFFFFFFU);
        borrow = (ai < borrow) ? 1 : 0;
    }
    while (a->len > 0 && a->limb[a->len - 1] == 0) a->len--;
}

/* ---------- caching of sequence values ---------- */
static idx_t cache_len = 0;    /* number of stored terms */
static idx_t cache_cap = 0;    /* allocated capacity */
static seq_t *seq_cache = NULL;/* stored values as seq_t */

/* ensure capacity for at least 'need' terms */
static void cache_ensure(size_t need) {
    if (need <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap << 1 : 16;
    while (newcap < need) newcap <<= 1;
    void *tmp = realloc(seq_cache, newcap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = (seq_t *)tmp;
    cache_cap = newcap;
}

/* ---------- algorithm state ---------- */
static bi_t root2, rem2, root3, rem3, tmp;
static bool algo_init = false;
static idx_t last_k = 0;

/* ---------- function required by build ---------- */
seq_t a(idx_t n)
{
    /* initialize algorithm state once */
    if (!algo_init) {
        bi_init(&root2);  bi_init(&rem2);
        bi_init(&root3);  bi_init(&rem3);
        bi_init(&tmp);
        bi_set_uint32(&root2, 1);      /* floor(sqrt(2))   */
        bi_set_uint32(&rem2, 1);       /* 2 - 1^2          */
        bi_set_uint32(&root3, 1);      /* floor(sqrt(3))   */
        bi_set_uint32(&rem3, 2);       /* 3 - 1^2          */
        algo_init = true;
    }

    /* generate terms until we have at least n entries */
    while (cache_len < n) {
        /* next position (1‑based) */
        ++last_k;

        /* ----- compute next digit of sqrt(2) ----- */
        bi_shift_left(&rem2, 2);           /* rem2 *= 4 */
        bi_copy(&tmp, &root2);
        bi_shift_left(&tmp, 2);            /* tmp = root2 << 2 */
        bi_add_small(&tmp, 1);
        int bit2;
        if (bi_cmp(&rem2, &tmp) >= 0) {
            bi_sub_assign(&rem2, &tmp);
            bit2 = 1;
        } else {
            bit2 = 0;
        }
        bi_shift_left(&root2, 1);           /* root2 <<= 1 */
        if (bit2) root2.limb[0] |= 1;       /* root2 |= 1 */

        /* ----- compute next digit of sqrt(3) ----- */
        bi_shift_left(&rem3, 2);           /* rem3 *= 4 */
        bi_copy(&tmp, &root3);
        bi_shift_left(&tmp, 2);            /* tmp = root3 << 2 */
        bi_add_small(&tmp, 1);
        int bit3;
        if (bi_cmp(&rem3, &tmp) >= 0) {
            bi_sub_assign(&rem3, &tmp);
            bit3 = 1;
        } else {
            bit3 = 0;
        }
        bi_shift_left(&root3, 1);           /* root3 <<= 1 */
        if (bit3) root3.limb[0] |= 1;       /* root3 |= 1 */

        /* store if bits coincide */
        if (bit2 == bit3) {
            cache_ensure(cache_len + 1);
            seq_cache[cache_len++] = (seq_t)last_k;
        }
    }

    return seq_cache[n - 1];
}
