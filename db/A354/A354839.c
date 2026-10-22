/*
 * A354839  Beginning with 0, smallest positive integer not yet in the
 *          sequence such that the concatenation of two digits of the sequence
 *          separated by a comma is prime.
 * https://oeis.org/A354839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   first_digit_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is defined recursively:
 *   a(0) = 0.
 *   For n > 0 let d be the last decimal digit of a(n‑1).
 *   a(n) is the smallest positive integer not already in the
 *   sequence such that the two‑digit number d·10 + f is prime,
 *   where f is the first decimal digit of that integer.  */

/* ---------- standard headers ---------- */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/first_digit_seq.h"

/* ---------- primitive prime test (for two‑digit numbers) ---------- */
static inline bool is_small_prime(int x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (int d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* ---------- dynamic storage for the sequence ---------- */
static seq_t *seq      = NULL;  /* cached sequence values   */
static size_t seq_cap  = 0;     /* number of stored values */
static unsigned char *used   = NULL;  /* bit‑set for used indices */
static size_t       used_cap = 0;     /* capacity of 'used' */

/* ---------- ensure used array is large enough ---------- */
static void ensure_used_cap(idx_t needed) {
    if ((idx_t)used_cap > needed) return;
    size_t new_cap = used_cap ? used_cap * 2 : 1;
    if (new_cap <= (size_t)needed) new_cap = (size_t)needed + 1;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
    if (!tmp) abort();  /* memory allocation failed */
    /* new area is uninitialised; set to 0 */
    if (new_cap > used_cap)
        memset(tmp + used_cap, 0, new_cap - used_cap);
    used = tmp;
    used_cap = new_cap;
}

/* ---------- main function ---------- */
seq_t a(idx_t n) {
    /* Initialise on first call */
    if (seq == NULL) {
        seq_cap = 1;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) abort();
        seq[0]    = (seq_t)0;  /* a(0) = 0 */

        used_cap = 1;
        used = (unsigned char *)malloc(used_cap * sizeof(unsigned char));
        if (!used) abort();
        used[0] = 1;          /* 0 is considered used */
    }

    /* Return cached value if already computed */
    if (n < seq_cap) return seq[n];

    /* Compute values up to index n */
    int prev_last = (int)(seq[seq_cap - 1] % 10);

    while (seq_cap <= n) {
        /* Find the smallest unused candidate satisfying the prime test */
        idx_t cand = 1;  /* search starts from 1 each time */
        for (;;) {
            ensure_used_cap(cand);
            if (used[cand]) { cand++; continue; }           /* already used */

            int fd = first_digit_seq((seq_t)cand);
            int concat = prev_last * 10 + fd;
            if (!is_small_prime(concat)) { cand++; continue; }

            /* Candidate found */
            used[cand] = 1;
            seq = (seq_t *)realloc(seq, (seq_cap + 1) * sizeof(seq_t));
            if (!seq) abort();
            seq[seq_cap++] = (seq_t)cand;

            prev_last = (int)(cand % 10);  /* update last digit for next step */
            break;
        }
    }

    return seq[n];
}
