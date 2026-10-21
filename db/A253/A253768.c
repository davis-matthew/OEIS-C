/*
 * A253768  Partial sums of A098550.
 * https://oeis.org/A253768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A253768 – Partial sums of A098550
 *  a(1)=1, a(2)=3, a(3)=6, a(4)=10, a(5)=19, ...
 *  a(n) = sum_{k=1..n} A098550(k)
 *
 *  A098550 is defined by:
 *      a(1)=1, a(2)=2, a(3)=3;
 *      for n>3 a(n) is the smallest integer not yet used
 *          that is coprime with a(n‑1) and shares a common factor with a(n‑2).
 *
 *  This implementation keeps a static cache of the sequence and the set
 *  of used integers.  The cache grows automatically when larger n
 *  values are requested.  The algorithm runs in O(n^2) time but is
 *  fast enough for n up to about 10 000 and respects all the type limits
 *  for seq_t and idx_t.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"

/* Static cache of A098550 values */
static seq_t  *seq      = NULL;    /* array of generated terms        */
static size_t seq_cap   = 0;       /* current capacity of seq[]      */
static size_t seq_len   = 0;       /* number of terms generated      */

/* Static bitset of used integers */
static uint8_t *used     = NULL;    /* used[i] == 1  <=>  i already used       */
static size_t  used_cap  = 0;       /* current capacity of used[]        */

/* Ensure that seq has capacity for at least 'need' terms.  The array
 * is reallocated using the language–appropriate allocator so that any
 * constructor of seq_t is invoked.  Existing elements are copied. */
static void
ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;

    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;

#ifdef __cplusplus
    seq_t *new_ptr = new seq_t[new_cap];
#else
    seq_t *new_ptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif

    if (seq) {
        for (size_t i = 0; i < seq_len; ++i) new_ptr[i] = seq[i];
#ifdef __cplusplus
        delete [] seq;
#else
        free(seq);
#endif
    }
    seq = new_ptr;
    seq_cap = new_cap;
}

/* Ensure that used[] can index at least one more integer 'upto'. */
static void
ensure_used_cap(idx_t upto)
{
    if ((size_t)upto + 1 <= used_cap) return;

    size_t new_cap = used_cap ? used_cap << 1 : 256;
    while (new_cap <= (size_t)upto) new_cap <<= 1;

#ifdef __cplusplus
    uint8_t *new_ptr = new uint8_t[new_cap];
#else
    uint8_t *new_ptr = (uint8_t *)malloc(new_cap * sizeof(uint8_t));
#endif

    if (used) {
        memcpy(new_ptr, used, used_cap * sizeof(uint8_t));
#ifdef __cplusplus
        delete [] used;
#else
        free(used);
#endif
    } else {
        memset(new_ptr, 0, new_cap * sizeof(uint8_t));
    }

    used = new_ptr;
    used_cap = new_cap;
}

/* Generate the next term of A098550 and append it to seq[]. */
static void
generate_next(void)
{
    seq_t prev1 = seq[seq_len - 1];
    seq_t prev2 = seq[seq_len - 2];
    idx_t m = 1;

    for (;;) {
        ensure_used_cap(m);
        if (used[m]) {
            ++m;
            continue;
        }

        seq_t mt = (seq_t)m;
        if (gcd(mt, prev1) == 1 && gcd(mt, prev2) > 1) {
            used[m] = 1;
            ensure_seq_cap(seq_len + 1);
            seq[seq_len++] = mt;
            break;
        }
        ++m;
    }
}

/* Public function required by the build system.  It returns the
 * partial sum of the first n terms of A098550.  The function is
 * thread-unsafe and caches results across calls. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* the OEIS offset is 1, but 0 is harmless */

    /* Initialise the cache on first call */
    if (seq_len == 0) {
        seq_cap = 8;
#ifdef __cplusplus
        seq = new seq_t[seq_cap];
#else
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
#endif
        seq_len = 3;
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        seq[2] = (seq_t)3;

        used_cap = 256;
#ifdef __cplusplus
        used = new uint8_t[used_cap];
#else
        used = (uint8_t *)malloc(used_cap * sizeof(uint8_t));
#endif
        memset(used, 0, used_cap * sizeof(uint8_t));
        used[1] = used[2] = used[3] = 1;
    }

    /* Extend the cache until it contains n terms */
    while (seq_len < n) {
        generate_next();
    }

    /* Compute the partial sum a(n) */
    seq_t sum = 0;
    for (size_t i = 0; i < n; ++i) {
        sum += seq[i];
    }
    return sum;
}
