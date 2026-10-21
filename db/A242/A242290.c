/*
 * A242290  Positive integers k for which the k-th tetrahedral number is
 *          greedy-summable.
 * https://oeis.org/A242290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer k for which the k-th
 *            tetrahedral number is greedy‑summable
 * Range:     u8 n <= 6, u16 n <= 47, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


static seq_t *tetra = NULL;      /* 1‑based tetrahedral numbers, tetra[0] unused */
static size_t tetra_len = 0;     /* next index to fill (1‑based) */
static size_t tetra_cap = 0;

static seq_t *found = NULL;      /* stored k’s, 1‑based */
static size_t found_len = 0;     /* number of k’s found so far */
static size_t found_cap = 0;

static idx_t last_k = 0;         /* last k processed */

/* Make sure tetra[k] is available (k >= 1). */
static void ensure_tetra(idx_t k)
{
    if ((size_t)k >= tetra_cap) {
        size_t new_cap = tetra_cap ? tetra_cap << 1 : 64;
        while (new_cap <= (size_t)k)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(tetra, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        tetra = tmp;
        tetra_cap = new_cap;
    }
    while (tetra_len <= (size_t)k) {
        idx_t i = (idx_t)tetra_len;
        seq_t val = (seq_t)i * (i + 1) * (i + 2) / 6;
        tetra[i] = val;
        ++tetra_len;
    }
}

/* Test whether T_k is greedy‑summable using distinct earlier tetrahedral numbers. */
static bool is_greedy(idx_t k)
{
    seq_t rem = tetra[k];
    for (idx_t j = k ? k - 1 : 0; j > 0; --j) {
        seq_t tj = tetra[j];
        if (tj <= rem) {
            rem -= tj;
            if (rem == 0)
                return true;
        }
    }
    return false;
}

/* Ensure the found array can hold at least 'needed' elements. */
static void grow_found(size_t needed)
{
    if (found_cap < needed) {
        size_t new_cap = found_cap ? found_cap << 1 : 64;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(found, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        found = tmp;
        found_cap = new_cap;
    }
}

/* Return the n-th term of A242290. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n = 0, but harmless */

    while (found_len < (size_t)n) {
        ++last_k;
        ensure_tetra(last_k);
        if (is_greedy(last_k)) {
            grow_found(found_len + 1);
            found[found_len++] = (seq_t)last_k;
        }
    }
    return found[n - 1];
}
