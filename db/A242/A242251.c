/*
 * A242251  Smallest positive number divisible by n which is not of the form
 *          a(i), a(i)+a(j), or |a(i)-a(j)| for i and j <= n-1.
 * https://oeis.org/A242251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 868, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code below implements OEIS A242251:
   a(n) = smallest positive integer divisible by n which is
   NOT equal to any previously produced term a(i),
   TO any sum a(i)+a(j), or to any absolute difference |a(i)-a(j)|
   for 1 ≤ i, j ≤ n-1.  The function is memoised so that calls
   to a() with arbitrary n are answered in O(1) after the value
   has been generated.  The algorithm performs a simple search
   for the smallest admissible multiple of n and marks all
   forbidden numbers in a dynamically resizing bitset.  It works
   for any seq_t type that can be stored in a bit position
   (i.e. fits within size_t).  The code is pure C11/C++17.  */

/* --- required includes ---------------------------------------------- */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* --- static storage for the sequence and forbidden numbers ---------- */
static seq_t *seq_terms   = NULL;   /* 1‑based array: seq_terms[i] = a(i) */
static size_t seq_cap     = 0;       /* current capacity of seq_terms  */

static unsigned char *banned_bits = NULL; /* bitset for forbidden values */
static size_t banned_bits_len     = 0;    /* number of bits currently allocated */

/* -------------------------------------------------------------------- */
/* Ensure seq_terms can hold index idx (1‑based).                       */
static void ensure_seq_terms(size_t idx)
{
    if (idx <= seq_cap) return;

    size_t new_cap = seq_cap ? seq_cap << 1 : 128;
    while (new_cap < idx) new_cap <<= 1;

    seq_t *new_terms = (seq_t *)calloc(new_cap + 1, sizeof(seq_t));
    if (!new_terms) abort();

    /* copy existing terms */
    if (seq_terms) {
        memcpy(new_terms, seq_terms, (seq_cap + 1) * sizeof(seq_t));
        free(seq_terms);
    }

    seq_terms = new_terms;
    seq_cap   = new_cap;
}

/* -------------------------------------------------------------------- */
/* Ensure that bit idx (0‑based) exists in the banned_bits bitset.      */
static void ensure_banned_bits(size_t idx)
{
    if (idx < banned_bits_len) return;

    size_t new_bits = banned_bits_len ? banned_bits_len << 1 : 64;
    while (new_bits <= idx) new_bits <<= 1;

    size_t new_bytes = (new_bits + 7) / 8;
    unsigned char *tmp = (unsigned char *)realloc(banned_bits, new_bytes);
    if (!tmp) abort();

    /* initialise newly allocated area to zero */
    if (new_bytes > banned_bits_len / 8) {
        memset(tmp + (banned_bits_len / 8), 0,
               new_bytes - (banned_bits_len / 8));
    }

    banned_bits     = tmp;
    banned_bits_len = new_bits;      /* store number of bits */
}

/* -------------------------------------------------------------------- */
/* Mark bit idx as forbidden.                                           */
static void set_banned(size_t idx)
{
    ensure_banned_bits(idx);
    banned_bits[idx >> 3] |= (unsigned char)(1u << (idx & 7));
}

/* -------------------------------------------------------------------- */
/* Test whether number idx is forbidden.                               */
static inline bool is_banned(size_t idx)
{
    if (idx >= banned_bits_len) return false;
    return (banned_bits[idx >> 3] >> (idx & 7)) & 1u;
}

/* -------------------------------------------------------------------- */
/* Main sequence generator                                               */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* out of bounds – harmless */

    /* ensure storage exists */
    ensure_seq_terms((size_t)n);

    static idx_t computed = 0;    /* highest index already generated */

    while (computed < n) {
        ++computed;
        idx_t cur = computed;

        seq_t cand = (seq_t)cur;          /* first positive multiple of cur */

        /* search for the first admissible multiple */
        while (is_banned((size_t)cand))
            cand += (seq_t)cur;

        seq_terms[cur] = cand;            /* store the term */

        /* forbid the term itself for future indices */
        set_banned((size_t)cand);

        /* forbid sums and absolute differences with all previous terms */
        for (idx_t j = 1; j < cur; ++j) {
            seq_t sum = seq_terms[j] + seq_terms[cur];
            set_banned((size_t)sum);

            seq_t diff = (seq_terms[j] > seq_terms[cur])
                         ? seq_terms[j] - seq_terms[cur]
                         : seq_terms[cur] - seq_terms[j];
            set_banned((size_t)diff);
        }

        /* also forbid the double of the new term (self‑sum) */
        seq_t self_sum = seq_terms[cur] + seq_terms[cur];
        set_banned((size_t)self_sum);
    }

    return seq_terms[(size_t)n];
}
