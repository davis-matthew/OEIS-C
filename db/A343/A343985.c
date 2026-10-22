/*
 * A343985  a(n) = A343984(n)/2.
 * https://oeis.org/A343985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n‑digit singular subwords of the Thue–Morse
 *            word ending with 0
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm generates a sufficiently long prefix of the Thue–Morse word,
   constructs hash tables for all length‑n and length-(n+1) subwords,
   deduplicates the length‑n subwords, and tests for singularity.
   All arithmetic is performed modulo 2^64, using two independent
   rolling hashes to reduce collision probability. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Rolling‑hash parameters (any pair of distinct large bases works) */
static const uint64_t B1 = 911382323ull;
static const uint64_t B2 = 972663749ull;

/* --------------------------------------------------------------------- */
/* Simple structures for the two hash tables */
typedef struct { uint64_t h1, h2; } Hash;

typedef struct { uint64_t h1, h2; idx_t pos; } HashPos;

/* --------------------------------------------------------------------- */
/* Helper to compute ceil_log2(x).  x >= 1 */
static idx_t ceil_log2_u64(idx_t x) {
    idx_t k = 0;
    /* shift until 2^k >= x */
    while ((1ull << k) < x) ++k;
    return k;
}

/* --------------------------------------------------------------------- */
/* Thue‑Morse bit: parity of 1‑bits of i */
static inline uint8_t tm_bit(idx_t i) {
    return (uint8_t)__builtin_parityll(i);
}

/* --------------------------------------------------------------------- */
/* Comparator for qsort on Hash (lexicographic on h1, then h2) */
static int cmp_hash(const void *a, const void *b) {
    const Hash *ha = (const Hash *)a;
    const Hash *hb = (const Hash *)b;
    if (ha->h1 < hb->h1) return -1;
    if (ha->h1 > hb->h1) return  1;
    if (ha->h2 < hb->h2) return -1;
    if (ha->h2 > hb->h2) return  1;
    return 0;
}

/* Comparator for HashPos (ignores pos) */
static int cmp_hashpos(const void *a, const void *b) {
    const HashPos *ha = (const HashPos *)a;
    const HashPos *hb = (const HashPos *)b;
    if (ha->h1 < hb->h1) return -1;
    if (ha->h1 > hb->h1) return  1;
    if (ha->h2 < hb->h2) return -1;
    if (ha->h2 > hb->h2) return  1;
    return 0;
}

/* Binary search in a sorted array of Hash */
static inline int has_hash(const Hash *arr, size_t n,
                           uint64_t h1, uint64_t h2) {
    size_t l = 0, r = n;
    while (l < r) {
        size_t m = (l + r) >> 1;
        if (arr[m].h1 < h1 ||
           (arr[m].h1 == h1 && arr[m].h2 < h2))
            l = m + 1;
        else
            r = m;
    }
    return (l < n && arr[l].h1 == h1 && arr[l].h2 == h2);
}

/* --------------------------------------------------------------------- */
/* Main function required by the build system */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1; outside spec */

    /* ----------------------------------------------------------------- */
    /* Determine a conservative prefix length:
       L = 2^(ceil_log2(n+1)+2)   (always ≥ 4*(n+1) for n≥1) */
    idx_t len_np1 = n + 1;
    idx_t k = ceil_log2_u64(len_np1);
    idx_t L = (idx_t)1 << (k + 2);          /* 2^(k+2) */

    /* ----------------------------------------------------------------- */
    /* Allocate temporary buffers */
    uint8_t  *g      = (uint8_t *)malloc((size_t)L);
    uint64_t *pref1  = (uint64_t *)malloc((size_t)(L + 1) * sizeof(uint64_t));
    uint64_t *pref2  = (uint64_t *)malloc((size_t)(L + 1) * sizeof(uint64_t));
    uint64_t *pow1   = (uint64_t *)malloc((size_t)(len_np1 + 1) * sizeof(uint64_t));
    uint64_t *pow2   = (uint64_t *)malloc((size_t)(len_np1 + 1) * sizeof(uint64_t));
    if (!g || !pref1 || !pref2 || !pow1 || !pow2) {
        /* memory error – return 0 (should not happen in tests) */
        free(g); free(pref1); free(pref2); free(pow1); free(pow2);
        return (seq_t)0;
    }

    /* ----------------------------------------------------------------- */
    /* Build the Thue‑Morse word of length L */
    for (idx_t i = 0; i < L; ++i)
        g[i] = tm_bit(i);

    /* ----------------------------------------------------------------- */
    /* Prefix hashes and powers of the bases */
    pref1[0] = pref2[0] = 0;
    pow1[0] = pow2[0] = 1;
    for (idx_t i = 0; i < L; ++i) {
        pref1[i + 1] = pref1[i] * B1 + (uint64_t)(g[i] + 1);
        pref2[i + 1] = pref2[i] * B2 + (uint64_t)(g[i] + 1);
        if (i < len_np1) {  /* pow arrays only up to len_np1 */
            pow1[i + 1] = pow1[i] * B1;
            pow2[i + 1] = pow2[i] * B2;
        }
    }

    /* ----------------------------------------------------------------- */
    /* All subwords of length (n+1) */
    size_t Nnp1 = (size_t)(L - len_np1 + 1);
    Hash *h_np1 = (Hash *)malloc(Nnp1 * sizeof(Hash));
    if (!h_np1) {
        free(g); free(pref1); free(pref2); free(pow1); free(pow2);
        return (seq_t)0;
    }
    for (idx_t i = 0; i < Nnp1; ++i) {
        h_np1[i].h1 = pref1[i + len_np1] - pref1[i] * pow1[len_np1];
        h_np1[i].h2 = pref2[i + len_np1] - pref2[i] * pow2[len_np1];
    }
    qsort(h_np1, Nnp1, sizeof(Hash), cmp_hash);

    /* ----------------------------------------------------------------- */
    /* All subwords of length n (with associated start index) */
    size_t Nn = (size_t)(L - n + 1);
    HashPos *h_n = (HashPos *)malloc(Nn * sizeof(HashPos));
    if (!h_n) {
        free(g); free(pref1); free(pref2); free(pow1); free(pow2); free(h_np1);
        return (seq_t)0;
    }
    for (idx_t i = 0; i < Nn; ++i) {
        h_n[i].h1 = pref1[i + n] - pref1[i] * pow1[n];
        h_n[i].h2 = pref2[i + n] - pref2[i] * pow2[n];
        h_n[i].pos = i;
    }
    qsort(h_n, Nn, sizeof(HashPos), cmp_hashpos);

    /* ----------------------------------------------------------------- */
    /* Count distinct length‑n subwords ending with 0 that are singular */
    seq_t count = 0;
    bool first = true;
    uint64_t prev_h1 = 0, prev_h2 = 0;

    for (size_t idx = 0; idx < Nn; ++idx) {
        const HashPos *hp = &h_n[idx];
        /* Skip duplicates */
        if (!first && hp->h1 == prev_h1 && hp->h2 == prev_h2)
            continue;
        first = false;
        prev_h1 = hp->h1;
        prev_h2 = hp->h2;

        /* Must end with 0 */
        if (g[hp->pos + n - 1] != 0)
            continue;

        /* Check existence of the two extensions */
        uint64_t h1_w0 = hp->h1 * B1 + 1ull;
        uint64_t h2_w0 = hp->h2 * B2 + 1ull;
        uint64_t h1_w1 = hp->h1 * B1 + 2ull;
        uint64_t h2_w1 = hp->h2 * B2 + 2ull;

        int ex0 = has_hash(h_np1, Nnp1, h1_w0, h2_w0);
        int ex1 = has_hash(h_np1, Nnp1, h1_w1, h2_w1);

        if (ex0 ^ ex1) ++count;
    }

    /* ----------------------------------------------------------------- */
    /* Clean up and return */
    free(g); free(pref1); free(pref2); free(pow1); free(pow2);
    free(h_np1); free(h_n);
    return count;
}
