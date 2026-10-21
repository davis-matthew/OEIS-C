/*
 * A299636  Solution (c(n)) of the system of 3 complementary equations in
 *          Comments.
 * https://oeis.org/A299636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, b(0)=2; for n≥1
 *   a(n)=least new k ≥ 2·b(n-1)
 *   b(n)=least new k
 *   c(n)=a(n)+b(n)
 * The function below returns c(n). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Static storage for the sequences and a bitset of used integers. */
static seq_t *a_seq = NULL, *b_seq = NULL, *c_seq = NULL;
static size_t seq_cap = 0;          /* capacity of the array tables           */
static uint8_t *used = NULL;        /* bitset: used[i]==1 if i already used  */
static size_t used_cap = 0;         /* capacity (bytes) of the used bitset    */
static idx_t last_computed = (idx_t)-1;
static idx_t next_unused = 1;       /* smallest integer not yet used         */
static bool initialized = false;

/* Ensure that the sequence arrays can hold at least indices 0…need. */
static void ensure_seq_cap(idx_t need) {
    if ((idx_t)seq_cap <= need) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 32;
        while ((idx_t)new_cap <= need)
            new_cap *= 2;
        seq_t *new_a = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
        seq_t *new_b = (seq_t *)realloc(b_seq, new_cap * sizeof(seq_t));
        seq_t *new_c = (seq_t *)realloc(c_seq, new_cap * sizeof(seq_t));
        if (!new_a || !new_b || !new_c)
            abort();  /* out‑of‑memory */
        a_seq = new_a;
        b_seq = new_b;
        c_seq = new_c;
        seq_cap = new_cap;
    }
}

/* Ensure that the used bitset covers index ‘need’. */
static void ensure_used_cap(idx_t need) {
    if ((size_t)used_cap <= (size_t)need) {
        size_t new_cap = used_cap ? used_cap * 2 : 128;
        while ((size_t)new_cap <= (size_t)need)
            new_cap *= 2;
        uint8_t *new_arr = (uint8_t *)realloc(used, new_cap);
        if (!new_arr)
            abort();
        if (new_cap > used_cap)
            memset(new_arr + used_cap, 0, new_cap - used_cap);
        used = new_arr;
        used_cap = new_cap;
    }
}

/* Return the n‑th term c(n). */
seq_t a(idx_t n) {
    /* One‑time initialization. */
    if (!initialized) {
        initialized = true;
        ensure_seq_cap(0);
        ensure_used_cap(3);                 /* need indices 1,2,3 for base case */

        a_seq[0] = (seq_t)1;
        b_seq[0] = (seq_t)2;
        c_seq[0] = a_seq[0] + b_seq[0];

        used[1] = 1;
        used[2] = 1;
        used[3] = 1;

        next_unused = 4;                   /* smallest unused after the base case */
        last_computed = 0;
    }

    /* Compute up to the requested n if needed. */
    if (n > last_computed) {
        idx_t i;
        ensure_seq_cap(n);
        for (i = last_computed + 1; i <= n; ++i) {
            /* ----- Compute a(i) ----- */
            seq_t need = b_seq[i - 1] * (seq_t)2;      /* 2 * b(i-1) */
            idx_t idx_need = (idx_t)need;
            ensure_used_cap(idx_need);
            while (used[idx_need]) {
                ++need;
                ++idx_need;
                ensure_used_cap(idx_need);
            }
            a_seq[i] = need;
            used[idx_need] = 1;      /* mark as used */

            /* ----- Compute b(i) ----- */
            while (true) {
                ensure_used_cap(next_unused);
                if (!used[next_unused])
                    break;
                ++next_unused;
            }
            b_seq[i] = (seq_t)next_unused;
            used[next_unused] = 1;
            ++next_unused;           /* advance for the next round */

            /* ----- Compute c(i) ----- */
            c_seq[i] = a_seq[i] + b_seq[i];
            idx_t idx_c = (idx_t)c_seq[i];
            ensure_used_cap(idx_c);
            used[idx_c] = 1;
        }
        last_computed = n;
    }

    return c_seq[n];
}
