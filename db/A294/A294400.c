/*
 * A294400  Solution of the complementary equation a(n) = a(n-1) + b(n-2) + n,
 *          where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4.
 * https://oeis.org/A294400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Complementary sequences defined by
 *   a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4,
 *   a(n) = a(n-1) + b(n-2) + n      (n ≥ 2)
 * This function returns a(n) for any n ≥ 0, memoising results and
 * generating the complementary sequence b() on demand.  It uses only
 * integer arithmetic and a simple bit‑set to keep track of values that
 * have already appeared.  The code compiles as C11 and as C++17.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* dynamic storage for the sequences and the used‑value bitset        */
/* ------------------------------------------------------------------ */
static seq_t *a_arr  = NULL;   /* values of a() */
static seq_t *b_arr  = NULL;   /* values of b() */
static size_t a_cap  = 0;      /* capacity of a_arr  */
static size_t b_cap  = 0;      /* capacity of b_arr  */
static size_t a_len  = 0;      /* number of computed a() entries */
static size_t b_len  = 0;      /* number of computed b() entries */

static uint64_t *bits          = NULL;   /* bit‑set of used integers */
static size_t    bit_words     = 0;       /* number of 64‑bit words in bits */

static uint64_t next_missing = 1;        /* candidate for the next unused integer */

static int initialized = 0;              /* true after first call */

/* ------------------------------------------------------------------ */
/* bit‑set helpers                                                  */
/* ------------------------------------------------------------------ */
static inline int used(uint64_t v)
{
    uint64_t w = v >> 6;                 /* word index          */
    if (w >= bit_words) return 0;         /* word not allocated  */
    return (int)((bits[w] >> (v & 63)) & 1ULL);
}

static void set_used(uint64_t v)
{
    uint64_t w = v >> 6;                 /* word index          */
    if (w >= bit_words) {
        size_t new_words = w + 1;
        /* grow bit‑set */
        uint64_t *new_ptr = (uint64_t *)realloc(bits, new_words * sizeof(uint64_t));
        if (!new_ptr) return;              /* allocation failed – ignore */
        memset(new_ptr + bit_words, 0,
               (new_words - bit_words) * sizeof(uint64_t));
        bits     = new_ptr;
        bit_words = new_words;
    }
    bits[w] |= (1ULL << (v & 63));
}

/* ------------------------------------------------------------------ */
/* main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* -------- initialisation -------------------------------------- */
    if (!initialized) {
        a_cap = 4;
        a_arr = (seq_t *)calloc(a_cap, sizeof(seq_t));
        b_cap = 4;
        b_arr = (seq_t *)calloc(b_cap, sizeof(seq_t));

        /* known initial values */
        a_arr[0] = (seq_t)1;
        a_arr[1] = (seq_t)3;
        a_len  = 2;

        b_arr[0] = (seq_t)2;
        b_arr[1] = (seq_t)4;
        b_len  = 2;

        /* mark used numbers 1,2,3,4 */
        set_used(1); set_used(2); set_used(3); set_used(4);
        next_missing = 1;
        initialized = 1;
    }

    /* -------- already known value --------------------------------- */
    if ((idx_t)n < a_len) return a_arr[n];

    /* -------- ensure capacity for a_arr ---------------------------- */
    if (a_cap == 0) a_cap = 4;
    while (a_cap <= (size_t)n) a_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_arr, a_cap * sizeof(seq_t));
    if (!tmp) return (seq_t)0;           /* allocation failure */
    a_arr = tmp;

    /* -------- compute values up to n ------------------------------ */
    for (idx_t i = a_len; i <= n; ++i) {
        /* generate missing elements of b() up to index i-2 */
        while (b_len <= (size_t)(i - 2)) {
            /* find next unused integer */
            while (used(next_missing)) ++next_missing;
            seq_t val = (seq_t)next_missing;
            set_used(next_missing);
            ++next_missing;

            /* grow b_arr if needed */
            if (b_cap == 0) b_cap = 4;
            if (b_len == b_cap) {
                b_cap <<= 1;
                seq_t *btmp = (seq_t *)realloc(b_arr,
                                              b_cap * sizeof(seq_t));
                if (!btmp) return (seq_t)0;   /* allocation failure */
                b_arr = btmp;
            }
            b_arr[b_len++] = val;
        }

        /* compute a(i) */
        seq_t ai = a_arr[i - 1] + b_arr[i - 2] + (seq_t)i;
        a_arr[i] = ai;
        set_used((uint64_t)ai);
    }

    a_len = (size_t)n + 1;
    return a_arr[n];
}
