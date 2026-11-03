/*
 * A326879  BII-numbers of connected connectedness systems.
 * https://oeis.org/A326879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th BII‑number of a connectedness system. A
 *            BII‑number is a non‑negative integer. The set of edges of the
 *            set‑system corresponding to BII‑number n is obtained by taking
 *            the bit positions of the 1‑bits of n (with the least significant
 *            bit counted as position 1). Each bit position p re...
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are defined by the build.                  */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* ------------------------------------------------------------------ */
static bool is_connected_bii(idx_t n)
{
    /* By definition n = 0 corresponds to the empty set.  It is
       considered connected in the OEIS entry. */
    if (n == 0)
        return true;

    /* edges are the bit positions of the 1‑bits of n; each edge
       is represented by the integer p = position (starting at 1).  */
    uint64_t edges[64];
    int m = 0;
    for (int pos = 0; n > 0; ++pos) {
        if (n & 1ULL) {
            edges[m++] = (uint64_t)(pos + 1);
        }
        n >>= 1;
    }

    /* Compute the union of all vertices. */
    uint64_t all = 0;
    for (int i = 0; i < m; ++i)
        all |= edges[i];

    /* Check that an edge equals the universal set. */
    bool has_universal = false;
    for (int i = 0; i < m; ++i) {
        if (edges[i] == all) {
            has_universal = true;
            break;
        }
    }
    if (!has_universal)
        return false;

    /* Closure: for every pair of overlapping edges, their union
       must also be present. */
    for (int i = 0; i < m; ++i) {
        for (int j = i + 1; j < m; ++j) {
            if (edges[i] & edges[j]) {
                uint64_t u = edges[i] | edges[j];
                bool found = false;
                for (int k = 0; k < m; ++k) {
                    if (edges[k] == u) {
                        found = true;
                        break;
                    }
                }
                if (!found)
                    return false;
            }
        }
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Helper: test whether a BII‑number n yields a connectedness system */

/* ------------------------------------------------------------------ */
/* Dynamic storage for the sequence values                            */
/* ------------------------------------------------------------------ */
static seq_t *seq   = NULL;   /* array of generated BII‑numbers      */
static size_t seq_cap = 0;    /* allocated capacity                   */
static size_t seq_len = 0;    /* number of terms stored              */
static idx_t next_candidate = 0; /* next integer to test                */

/* Append a value to the dynamic array, resizing if necessary. */
static void append_seq(idx_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 4;
        seq_t *new_arr = mem_resize(seq_t, seq, new_cap);
        if (!new_arr)
            exit(1);   /* fatal: out of memory */
        seq = new_arr;
        seq_cap = new_cap;
    }
    seq[seq_len++] = (seq_t)val;
}

/* ------------------------------------------------------------------ */
/* Public function: returns the n-th term of A326879.                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1, but safe */
    while (seq_len < n) {
        if (is_connected_bii(next_candidate))
            append_seq(next_candidate);
        ++next_candidate;
    }
    return seq[n - 1];
}
