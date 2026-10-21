/*
 * A276757  Infinite Fibonacci word on the alphabet {1,2,3,4,5}.
 * https://oeis.org/A276757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = A(A(A(k))) for some k a(n) = 2 if n = B(A(k))
 *            a(n) = 3 if n = A(B(k)) a(n) = 4 if n = A(A(B(k))) a(n) = 5 if n
 *            = B(B(k)) where A(k) = ⌊k φ⌋ (lower Wythoff sequence) and B(k) =
 *            k + A(k) = ⌊k φ²⌋ (upper Wythoff sequence). The implementation
 *            generates the Beatty sequences A and B laz...
 * Range:     u8 n <= 112, u16 n <= 112, u32 n <= 112, u64 n <= 112,
 *            u128 n <= 112, big any n
 * Helpers:   binsearch
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/binsearch.h"

/* ------------------------------------------------------------------ */
/*  Beatty sequence storage (static, lazily built)                     */
/* ------------------------------------------------------------------ */
static idx_t *A_vals = NULL;   /* A(k) values, 0‑based (A(1) at index 0)   */
static idx_t *B_vals = NULL;   /* B(k) values, 0‑based                    */
static idx_t A_len  = 0;       /* number of stored A values                */
static idx_t B_len  = 0;       /* number of stored B values                */
static idx_t seq_cap = 0;      /* capacity of the above arrays             */
static idx_t used_cap = 0;     /* capacity of the used table              */
static unsigned char *used = NULL; /* used[i] == 1 => i appears in A or B   */
static idx_t next_i   = 1;     /* next index k for generation             */
static idx_t cur_a    = 1;     /* next candidate for A                     */

/* ------------------------------------------------------------------ */
/*  Binary search for an integer array                               */

/* ------------------------------------------------------------------ */
/*  Inverse mapping for A and B                                       */
/* ------------------------------------------------------------------ */
static bool inverseA(idx_t x, idx_t *k)
{
    idx_t pos;
    if (binsearch(A_vals, A_len, x, &pos)) {
        *k = pos + 1;          /* sequence indices start at 1 */
        return true;
    }
    return false;
}

static bool inverseB(idx_t x, idx_t *k)
{
    idx_t pos;
    if (binsearch(B_vals, B_len, x, &pos)) {
        *k = pos + 1;
        return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/*  Ensure the Beatty sequences cover all numbers up to 'max_n'      */
/* ------------------------------------------------------------------ */
static void ensure_sequences(idx_t max_n)
{
    if (max_n < 1) return;

    /* initialise storage if needed */
    if (seq_cap == 0) {
        seq_cap = 1024;
        A_vals = (idx_t*)malloc(seq_cap * sizeof(idx_t));
        B_vals = (idx_t*)malloc(seq_cap * sizeof(idx_t));
    }

    if (used_cap == 0) {
        used_cap = 1024;
        used = (unsigned char*)calloc(used_cap + 1, sizeof(unsigned char));
    }

    /* grow used array to accommodate the largest number seen so far */
    while (next_i + cur_a > used_cap) {   /* worst case b_val = a_val + k */
        idx_t old_cap = used_cap;
        used_cap <<= 1;
        unsigned char *tmp = (unsigned char*)realloc(used, used_cap + 1);
        if (!tmp) exit(1);
        memset(tmp + old_cap + 1, 0, used_cap - old_cap);
        used = tmp;
    }

    /* generate pairs until both A(k) and B(k) exceed max_n         */
    while (true) {
        /* find next unused integer for A */
        while (cur_a <= used_cap && used[cur_a]) cur_a++;

        /* enlarge used table if cur_a or b_val goes beyond it      */
        while (cur_a > used_cap) {
            idx_t old_cap = used_cap;
            used_cap <<= 1;
            unsigned char *tmp = (unsigned char*)realloc(used, used_cap + 1);
            if (!tmp) exit(1);
            memset(tmp + old_cap + 1, 0, used_cap - old_cap);
            used = tmp;
        }

        idx_t a_val = cur_a;
        idx_t b_val = a_val + next_i;          /* B(k) = A(k) + k */

        /* enlarge used table for b_val if necessary */
        while (b_val > used_cap) {
            idx_t old_cap = used_cap;
            used_cap <<= 1;
            unsigned char *tmp = (unsigned char*)realloc(used, used_cap + 1);
            if (!tmp) exit(1);
            memset(tmp + old_cap + 1, 0, used_cap - old_cap);
            used = tmp;
        }

        /* mark the numbers as used */
        used[a_val] = 1;
        used[b_val] = 1;

        /* grow arrays if needed */
        if (A_len == seq_cap) {
            seq_cap <<= 1;
            A_vals = (idx_t*)realloc(A_vals, seq_cap * sizeof(idx_t));
            B_vals = (idx_t*)realloc(B_vals, seq_cap * sizeof(idx_t));
        }

        /* store current pair */
        A_vals[A_len] = a_val;
        B_vals[B_len] = b_val;
        A_len++; B_len++;

        /* next candidate for A and next index k */
        cur_a = a_val + 1;
        ++next_i;

        /* stop once both values exceed the needed range   */
        if (a_val > max_n && b_val > max_n)
            break;
    }
}

/* ------------------------------------------------------------------ */
/*  The required sequence function                                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* outside offset, harmless */

    /* ensure we have the Beatty sequences up to n   */
    ensure_sequences(n);

    /* type 1 : n = A(A(A(k))) */
    idx_t k1, k2, k3;
    if (inverseA(n, &k1) &&
        inverseA(k1, &k2) &&
        inverseA(k2, &k3)) {
        return (seq_t)1;
    }

    /* type 2 : n = B(A(k)) */
    idx_t p;
    if (inverseB(n, &p) && inverseA(p, &k1)) {
        return (seq_t)2;
    }

    /* type 3 : n = A(B(k)) */
    idx_t m;
    if (inverseA(n, &m) && inverseB(m, &p)) {
        return (seq_t)3;
    }

    /* type 4 : n = A(A(B(k))) */
    if (inverseA(n, &m) &&
        inverseA(m, &k1) &&
        inverseB(k1, &p)) {
        return (seq_t)4;
    }

    /* type 5 : n = B(B(k)) */
    if (inverseB(n, &p) && inverseB(p, &k1)) {
        return (seq_t)5;
    }

    /* Should never happen for n >= 1 */
    return (seq_t)0;
}
