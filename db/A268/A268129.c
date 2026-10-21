/*
 * A268129  If a(n) is a multiple of 3, then a(n+1) = a(n) + a(n-1).
 *          Lexicographic first permutation of the positive integers with this
 *          property.
 * https://oeis.org/A268129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 150, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A268129: Permutation of the positive integers with the rule
 *     if a(n) is a multiple of 3, then a(n+1) = a(n) + a(n-1).
 * The implementation generates the lexicographically first such
 * permutation.  Terms are cached so calls in arbitrary order are
 * efficient.  The code works for both C and C++ builds; seq_t may be
 * a builtin integer type or an arbitrary‑precision class.  Integer
 * indices use idx_t (uint64_t).  All bookkeeping is done with plain
 * dynamic arrays, so the code can compile in both languages.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Cached sequence terms, 0‑based (a(1) at index 0). */
static seq_t *seq_arr  = NULL;
static size_t seq_cap   = 0;       /* allocated capacity of seq_arr */
static size_t seq_len   = 0;       /* number of terms currently computed */

/* Boolean array marking whether a positive integer has already appeared. */
static unsigned char *used_arr = NULL;
static size_t used_cap   = 0;      /* allocated capacity of used_arr */

/* Smallest positive integer that has not yet been used. */
static idx_t next_unused = 1;

/* Ensure that seq_arr can hold at least 'n' terms (1‑based index). */
static void ensure_seq_cap(idx_t n) {
    if ((size_t)n > seq_cap) {
        size_t new_cap = seq_cap == 0 ? 8 : seq_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_arr = tmp;
        seq_cap = new_cap;
    }
}

/* Ensure that used_arr can be indexed by value 'val' (cast to size_t). */
static void ensure_used_cap(seq_t val) {
    size_t idx = (size_t)val;
    if (idx >= used_cap) {
        size_t new_cap = used_cap == 0 ? 8 : used_cap;
        while (new_cap <= idx) new_cap <<= 1;
        unsigned char *tmp = (unsigned char *)realloc(used_arr,
                                                      new_cap * sizeof(unsigned char));
        if (!tmp) abort();
        /* Zero the newly allocated portion. */
        memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(unsigned char));
        used_arr = tmp;
        used_cap = new_cap;
    }
}

/* Mark integer 'val' as used and update 'next_unused' appropriately. */
static void mark_used(seq_t val) {
    ensure_used_cap(val);
    size_t idx = (size_t)val;
    used_arr[idx] = 1;
    if (idx == (size_t)next_unused) {
        ++next_unused;
        while ((size_t)next_unused < used_cap && used_arr[next_unused]) ++next_unused;
    }
}

/* Return the smallest unused positive integer. */
static seq_t get_smallest_unused(void) {
    while ((size_t)next_unused < used_cap && used_arr[next_unused]) ++next_unused;
    seq_t val = next_unused;
    mark_used(val);
    return val;
}

/* Allocate used_arr with a reasonable initial size. */
static void init_used_arr(void) {
    if (!used_arr) {
        used_cap = 64;          /* Initial capacity */
        used_arr = (unsigned char *)calloc(used_cap, sizeof(unsigned char));
        if (!used_arr) abort();
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* safe guard – not used for the OEIS offset */

    /* Initialize used array when first called. */
    if (!used_arr) init_used_arr();

    ensure_seq_cap(n);

    /* Generate terms up to n lazily and cache them. */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 1;
        } else if (i == 2) {
            val = 2;
        } else if (i == 3) {
            val = 3;
        } else {
            seq_t prev      = seq_arr[i - 2];    /* a(i-1)  */
            seq_t prevprev  = seq_arr[i - 3];    /* a(i-2)  */
            if ((prev % 3) == 0) {
                /* Forced value: sum of previous two terms */
                val = prev + prevprev;
            } else {
                /* Choose the smallest unused integer to keep the sequence
                 * lexicographically minimal. */
                val = get_smallest_unused();
            }
        }
        seq_arr[i - 1] = val;
        seq_len = i;
        mark_used(val);
    }

    return seq_arr[n - 1];
}
