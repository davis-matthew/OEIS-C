/*
 * A350831  a(n) is the least positive integer not listed earlier such that
 *          its first digit together with a(n-1)'s last digit can form a
 *          prime; a(1) = 1.
 * https://oeis.org/A350831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least unused positive integer not a multiple of 10 such
 *            that its first digit together with a(n‑1)’s last digit can form
 *            a prime (either order). a(1) = 1. This definition is implemented
 *            directly by searching from the smallest unused candidate
 * Range:     u8 n <= 57, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Pre‑computed primality of all two‑digit numbers < 100 */
static bool prime_table[100] = {0};
static bool prime_table_inited = false;
static void init_prime_table(void)
{
    if (prime_table_inited) return;
    prime_table_inited = true;
    for (int n = 2; n < 100; ++n) {
        bool is_prime = true;
        for (int d = 2; d * d <= n; ++d) {
            if (n % d == 0) { is_prime = false; break; }
        }
        prime_table[n] = is_prime;
    }
}

/* Return the first (most significant) digit of a positive integer x. */
static inline seq_t first_digit(seq_t x)
{
    while (x >= 10) { x /= 10; }
    return x;
}

/* Test whether the two digits f and d can form a prime (either 10*f+d or 10*d+f). */
static bool can_form_prime(seq_t f, seq_t d)
{
    int n1 = (int)(10 * f + d);
    if (n1 >= 0 && n1 < 100 && prime_table[n1]) return true;
    int n2 = (int)(10 * d + f);
    return n2 >= 0 && n2 < 100 && prime_table[n2];
}

/* The requested sequence function. */
seq_t a(idx_t n)
{
    /* Persistent storage for the sequence and the set of used values. */
    static seq_t *seq    = NULL;   /* sequence values, 1‑based */
    static size_t seq_cap = 0;     /* capacity of seq array */
    static bool *used   = NULL;    /* flags of used numbers */
    static size_t used_cap = 0;    /* capacity of used array */

    init_prime_table();

    /* Ensure seq array has enough space for index n (1‑based). */
    size_t need_seq = (size_t)n + 1;
    if (seq_cap < need_seq) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        while (new_cap < need_seq) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        if (tmp != seq) {
            memset(tmp + seq_cap, 0, (new_cap - seq_cap) * sizeof(seq_t));
        } else {
            memset(tmp, 0, new_cap * sizeof(seq_t));
        }
        seq = tmp;
        seq_cap = new_cap;
    }

    /* Ensure used array can index all candidate numbers we will see. */
    if (used_cap < 16) {                 /* small initial size */
        size_t new_cap = 16;
        bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
        if (!tmp) abort();
        if (tmp != used) {
            memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
        } else {
            memset(tmp, 0, new_cap * sizeof(bool));
        }
        used = tmp;
        used_cap = new_cap;
    }

    /* Initialise a(1) if it hasn't been set yet. */
    if (seq[1] == 0) {
        seq[1] = (seq_t)1;
        if ((size_t)1 >= used_cap) {
            size_t new_cap = used_cap ? used_cap * 2 : 16;
            while (new_cap <= 1) new_cap *= 2;
            bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
            if (!tmp) abort();
            memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
            used = tmp;
            used_cap = new_cap;
        }
        used[1] = true;
    }

    /* Compute missing terms up to n. */
    for (size_t i = 2; i <= (size_t)n; ++i) {
        if (seq[i] != 0) continue;        /* already computed earlier */

        seq_t prev = seq[i - 1];
        seq_t d    = prev % 10;            /* last digit of previous term */

        seq_t cand = 1;
        for (;;) {
            /* Grow used array if needed for this candidate. */
            if ((size_t)cand >= used_cap) {
                size_t new_cap = used_cap ? used_cap * 2 : 16;
                while (new_cap <= (size_t)cand) new_cap *= 2;
                bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
                if (!tmp) abort();
                memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
                used = tmp;
                used_cap = new_cap;
            }

            /* Skip numbers that are multiples of 10. */
            if (cand % 10 == 0) { ++cand; continue; }

            /* Skip already used numbers. */
            if (used[(size_t)cand]) { ++cand; continue; }

            /* Candidate passes prime‑formation test? */
            seq_t f = first_digit(cand);
            if (can_form_prime(f, d)) {
                seq[i] = cand;
                used[(size_t)cand] = true;
                break;
            }
            ++cand;
        }
    }

    return seq[(size_t)n];
}
