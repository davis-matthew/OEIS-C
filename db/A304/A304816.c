/*
 * A304816  Solution (b(n)) of the complementary equation a(n) = b(4n) +
 *          b(5n); see Comments.
 * https://oeis.org/A304816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 228, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute complementary sequences a(n) and b(n) by iteratively
   selecting the least unused integer for b and marking a = b(4n)+b(5n).  */
/* The function 'a' returns the n-th term of OEIS A304816 (the b sequence). */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* static state */
static seq_t *B = NULL;                /* b(n) array */
static size_t Bcap = 0;                /* capacity of B */
static idx_t Bmax_seen = (idx_t)-1;    /* highest index of b computed */

static bool *used = NULL;              /* bitmap of used positive integers */
static size_t used_cap = 0;            /* capacity of used bitmap */
static uint64_t next_used_candidate = 1; /* next candidate integer */

/* grow the bitmap of used numbers up to v (inclusive) */
static void ensure_used_capacity(uint64_t v)
{
    if (v < used_cap) return;
    size_t new_cap = used_cap ? used_cap : 16;
    while ((size_t)v >= new_cap) new_cap <<= 1;
    bool *tmp = (bool*)realloc(used, new_cap * sizeof(bool));
    if (!tmp) abort();
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = new_cap;
}

/* grow the array B up to size idx (inclusive) */
static void ensure_B_capacity(size_t idx)
{
    if (idx < Bcap) return;
    size_t new_cap = Bcap ? Bcap : 16;
    while (new_cap <= idx) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(B, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    B = tmp;
    Bcap = new_cap;
}

/* compute b up to index 5*n inclusive, marking all a values that become available.
   This function may be called repeatedly; it only does extra work. */
static void compute_to(idx_t n)
{
    idx_t needBmax = (idx_t)5 * n;
    if (needBmax <= Bmax_seen) return;

    for (idx_t m = Bmax_seen + 1; m <= needBmax; ++m)
    {
        /* Ensure bitmap capacity for the candidate integer */
        ensure_used_capacity((uint64_t)next_used_candidate);

        /* Find the least unused positive integer */
        while (used[(size_t)next_used_candidate]) ++next_used_candidate;

        /* Assign b(m) and mark it used */
        seq_t valb = (seq_t)next_used_candidate;
        ensure_B_capacity((size_t)m);
        B[m] = valb;
        used[(size_t)next_used_candidate] = true;
        ++next_used_candidate;

        /* If m is 4k, try to compute a(k) when b(5k) already exists */
        if ((m % 4) == 0) {
            idx_t k = m / 4;
            if (5*k <= m) {
                seq_t vala = B[4*k] + B[5*k];
                uint64_t idx = (uint64_t)vala;
                ensure_used_capacity(idx);
                used[(size_t)idx] = true;
            }
        }

        /* If m is 5k, try to compute a(k) when b(4k) already exists */
        if ((m % 5) == 0) {
            idx_t k = m / 5;
            if (4*k <= m) {
                seq_t vala = B[4*k] + B[5*k];
                uint64_t idx = (uint64_t)vala;
                ensure_used_capacity(idx);
                used[(size_t)idx] = true;
            }
        }
    }

    Bmax_seen = needBmax;
}

/* Public function: returns the n-th term of the sequence A304816. */
seq_t a(idx_t n)
{
    /* Initialize static state on first call */
    static bool initialized = false;
    if (!initialized) {
        /* Allocate initial memory */
        Bcap = 16;
        B = (seq_t*)calloc(Bcap, sizeof(seq_t));
        if (!B) abort();

        used_cap = 16;
        used = (bool*)calloc(used_cap, sizeof(bool));
        if (!used) abort();

        /* Compute b(0) = 1 */
        ensure_used_capacity((uint64_t)1);
        B[0] = (seq_t)1;
        used[(size_t)1] = true;
        ++next_used_candidate;  /* next candidate is 2 */

        /* Compute a(0) = b(0)+b(0) = 2 and mark used */
        seq_t a0 = B[0] + B[0];
        ensure_used_capacity((uint64_t)a0);
        used[(size_t)a0] = true;
        ++next_used_candidate;  /* next candidate is 3 */

        Bmax_seen = 0;
        initialized = true;
    }

    compute_to(n);
    return B[n];
}
