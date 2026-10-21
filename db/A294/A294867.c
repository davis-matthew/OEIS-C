/*
 * A294867  Solution of the complementary equation a(n) = 2*a(n-1) - a(n-2) +
 *          b(n-1) -1, where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A294867 – Complementary sequences defined by
   a(0)=1, a(1)=2, b(0)=3 and
   a(n)=2*a(n-1)-a(n-2)+b(n-1)-1   (n≥2).
   The sequence a(n) is returned for any non‑negative index n.
   The implementation memoises computed terms and the complementary
   b(n) values, and keeps a bitmap of used numbers to determine the
   next unused value for b(n).
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- bitmap helpers ----------
   The bitmap is grown only as needed.  All conversions to/from
   seq_t are performed through idx_t, assuming the value fits in
   uint64_t for the required range. ---------- */

static void used_set(idx_t v, uint64_t **bits, size_t *size)
{
    size_t word = (size_t)(v >> 6);
    if (*bits == NULL) {
        /* start with a single word */
        *size = 1;
        *bits = (uint64_t*)malloc(*size * sizeof(uint64_t));
        (*bits)[0] = 0;
    } else if (word >= *size) {
        size_t newsize = *size;
        while (word >= newsize) newsize <<= 1;
        uint64_t *tmp = (uint64_t*)realloc(*bits, newsize * sizeof(uint64_t));
        if (!tmp) abort();
        for (size_t i = *size; i < newsize; ++i) tmp[i] = 0;
        *bits = tmp;
        *size = newsize;
    }
    (*bits)[word] |= (uint64_t)1 << (v & 63);
}

static bool used_get(idx_t v, uint64_t const *bits, size_t size)
{
    if (!bits) return false;
    size_t word = (size_t)(v >> 6);
    if (word >= size) return false;
    return ((bits[word] >> (v & 63)) & 1) != 0;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    /* Static state shared across calls (lazy init). */
    static seq_t *A = NULL;
    static seq_t *B = NULL;
    static uint64_t *used_bits = NULL;
    static size_t used_bits_size = 0;
    static idx_t computed = 0;   /* number of terms already known (a[0]..a[computed-1]) */

    /* Allocate/resize storage for A and B if needed. */
    if (!A) {
        size_t cap = (size_t)(n + 1);
        if (cap < 2) cap = 2;
        A = (seq_t*)malloc(cap * sizeof(seq_t));
        B = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!A || !B) abort();
        computed = 0;
    } else if ((size_t)computed <= n) {
        size_t cap = (size_t)(n + 1);
        if (cap > (size_t)computed) {
            A = (seq_t*)realloc(A, cap * sizeof(seq_t));
            B = (seq_t*)realloc(B, cap * sizeof(seq_t));
            if (!A || !B) abort();
        }
    }

    /* --- initialise missing starting values --- */
    if (computed == 0) {
        A[0] = (seq_t)1;
        B[0] = (seq_t)3;
        used_set((idx_t)1, &used_bits, &used_bits_size);
        used_set((idx_t)3, &used_bits, &used_bits_size);
        computed = 1;                     /* only a[0] known */
    }

    if (computed == 1) {
        A[1] = (seq_t)2;
        used_set((idx_t)2, &used_bits, &used_bits_size);
        /* find smallest unused > B[0] */
        idx_t cand = (idx_t)B[0] + 1;
        while (used_get(cand, used_bits, used_bits_size)) cand++;
        B[1] = (seq_t)cand;
        used_set(cand, &used_bits, &used_bits_size);
        computed = 2;
    }

    /* --- compute further terms up to requested n --- */
    while (computed <= n) {
        idx_t cur = computed;                    /* compute a[cur] */
        /* recurrence: a(cur) = 2*a(cur-1) - a(cur-2) + b(cur-1) - 1 */
        seq_t val = 2 * A[cur-1] - A[cur-2] + B[cur-1] - 1;
        A[cur] = val;
        used_set((idx_t)val, &used_bits, &used_bits_size);

        /* compute b[cur] (needed for future recurrences) */
        idx_t cand = (idx_t)B[cur-1] + 1;
        while (used_get(cand, used_bits, used_bits_size)) cand++;
        B[cur] = (seq_t)cand;
        used_set(cand, &used_bits, &used_bits_size);

        ++computed;
    }

    return A[n];
}
