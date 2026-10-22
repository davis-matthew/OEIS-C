/*
 * A384664  Solution of the complementary equation b(n)=a(a(n))+a(n)+3 with
 *          a(1)=1; this is the sequence b. Sequence a is A384663.
 * https://oeis.org/A384664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   b(n) = a(a(n)) + a(n) + 3, where a(1)=1 and * a(n) (for n>1) is
 *            the smallest positive integer not yet * appearing in
 *            b(1),…,b(n‑1). This function returns the n‑th * term of sequence
 *            A384664 (b(n)). The sequence is computed * lazily with
 *            memoisation; it works correctly for any n ≥ 1 * without assu...
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   grow_bool
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/grow_bool.h"

/* ---------- helper to grow a seq_t array (1‑based) ---------- */
static void grow_seq(size_t need, seq_t **arr, size_t *cap)
{
    if (need <= *cap) return;
    size_t nc = need + (need >> 1) + ((size_t)1024);
    *arr = (seq_t *)realloc(*arr, nc * sizeof(seq_t));
    if (!*arr) exit(1);
    memset(*arr + *cap, 0, (nc - *cap) * sizeof(seq_t));
    *cap = nc;
}

/* ---------- static state for the sequences ---------- */
static seq_t *B = NULL;           /* b(n) */
static size_t Bcap = 0;
static seq_t *A = NULL;           /* a(n)   (used for indices as well) */
static size_t Acap = 0;
static unsigned char *used_b = NULL;    /* 1 if value has appeared in b */
static size_t used_cap = 0;
static seq_t next_missing = 1;          /* smallest integer not yet used as b */
static idx_t computed_a_max = 0;        /* largest index computed for a */
static idx_t computed_b_max = 0;        /* largest index computed for b */

/* ---------- compute a(k) for all k up to maxIdx ---------- */
static void compute_a_up_to(idx_t maxIdx)
{
    while (computed_a_max < maxIdx) {
        ++computed_a_max;
        /* find smallest integer not yet in used_b */
        seq_t cand = next_missing;
        while (cand >= used_cap) grow_bool((size_t)cand + 1, &used_b, &used_cap);
        while (used_b[(size_t)cand]) {
            ++cand;
            if (cand >= used_cap) grow_bool((size_t)cand + 1, &used_b, &used_cap);
        }
        /* assign a[computed_a_max] */
        if (computed_a_max >= (idx_t)Acap) grow_seq((size_t)computed_a_max + 1, &A, &Acap);
        A[(size_t)computed_a_max] = cand;
        /* advance next_missing past the value just used */
        next_missing = cand + 1;
        while (next_missing >= used_cap) grow_bool((size_t)next_missing + 1, &used_b, &used_cap);
        while (used_b[(size_t)next_missing]) ++next_missing;
    }
}

/* ---------- public entry ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0; caller should not use */

    /* Ensure we have b[i] for all i ≤ n */
    if (n > computed_b_max) {
        if (n >= (idx_t)Bcap) grow_seq((size_t)n + 1, &B, &Bcap);

        for (idx_t m = computed_b_max + 1; m <= n; ++m) {
            /* Ensure a[m] is computed */
            compute_a_up_to(m);
            seq_t am   = A[(size_t)m];
            /* Ensure a[am] is computed (am is a value, may be larger than m) */
            compute_a_up_to((idx_t)am);
            seq_t aam  = A[(size_t)am];
            seq_t bm   = aam + am + 3;
            B[(size_t)m] = bm;

            /* Mark bm as used in b */
            size_t bi = (size_t)bm;
            if (bi >= used_cap) grow_bool(bi + 1, &used_b, &used_cap);
            used_b[bi] = 1;
            /* Advance next_missing if it hit the newly used bm */
            while (next_missing >= used_cap) grow_bool((size_t)next_missing + 1, &used_b, &used_cap);
            while (used_b[(size_t)next_missing]) ++next_missing;
        }
        computed_b_max = n;
    }

    return B[(size_t)n];
}
