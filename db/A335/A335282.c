/*
 * A335282  Lexicographically earliest strictly increasing sequence of
 *          positive integers whose Recamán transform has only distinct
 *          values.
 * https://oeis.org/A335282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the lexicographically earliest strictly increasing
 *            sequence of positive integers whose Recamán transform (defined
 *            in the OEIS entry) has only distinct values
 * Range:     u8 n <= 223, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static idx_t S_len = 0;          /* Number of computed a values so far */
static size_t A_cap = 0;
static seq_t *A_vals = NULL;     /* Array of a(n) values (1‑based indices) */

static size_t S_seen_cap = 0;    /* Capacity of seen array */
static size_t S_seen_len = 0;    /* Number of seen Recamán values */
static seq_t *S_seen = NULL;    /* Array of distinct r(n) values */

static seq_t R_prev = 0;         /* r(n-1) (Recamán value before the next step) */
static idx_t B_last = 0;         /* Last chosen b value (a(n-1)) */

/* Ensure A_vals has capacity for at least one more element. */
static void grow_A(void)
{
    if (S_len == A_cap) {
        size_t new_cap = A_cap ? A_cap << 1 : 64;
        seq_t *p = (seq_t *)realloc(A_vals, new_cap * sizeof(seq_t));
        if (!p) abort();
        A_vals = p;
        A_cap = new_cap;
    }
}

/* Ensure S_seen has capacity for one more element. */
static void grow_S(void)
{
    if (S_seen_len == S_seen_cap) {
        size_t new_cap = S_seen_cap ? S_seen_cap << 1 : 64;
        seq_t *p = (seq_t *)realloc(S_seen, new_cap * sizeof(seq_t));
        if (!p) abort();
        S_seen = p;
        S_seen_cap = new_cap;
    }
}

/* Append a value to the set of seen Recamán values. */
static void add_seen(seq_t v)
{
    grow_S();
    S_seen[S_seen_len++] = v;
}

/* Test whether v is already among the seen Recamán values. */
static bool seen_contains(seq_t v)
{
    for (size_t i = 0; i < S_seen_len; ++i)
        if (S_seen[i] == v) return true;
    return false;
}

/* Static initialisation of the state, executed lazily. */
static void init_state(void)
{
    if (S_len != 0) return;          /* already initialised */

    /* Allocate arrays. */
    A_vals = (seq_t *)malloc(64 * sizeof(seq_t));
    A_cap  = 64;
    S_seen = (seq_t *)malloc(64 * sizeof(seq_t));
    S_seen_cap = 64;

    /* Recamán start: r(0)=0 is already seen. */
    add_seen((seq_t)0);

    R_prev = (seq_t)0;
    B_last = 0;
}

/* Compute a(n).  The function may be called with n in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but safe */

    init_state();

    if (n <= S_len) return A_vals[n-1];

    /* Extend the state up to and including index n. */
    for (idx_t i = S_len + 1; i <= n; ++i) {
        /* Find the smallest possible candidate b > B_last. */
        idx_t cand_idx = B_last + 1;
        while (1) {
            seq_t cand = (seq_t)cand_idx;
            seq_t r_val;
            if (R_prev >= cand && !seen_contains(R_prev - cand)) {
                r_val = R_prev - cand;
            } else {
                r_val = R_prev + cand;
            }
            /* Ensure the new Recamán value is distinct. */
            if (!seen_contains(r_val)) {
                /* Accept this candidate. */
                grow_A();
                A_vals[S_len++] = cand;
                add_seen(r_val);
                R_prev = r_val;
                B_last = cand_idx;
                break;
            }
            ++cand_idx;                 /* try the next larger b */
        }
    }

    return A_vals[n-1];
}
