/*
 * A256148  Primitive prime factors of the cyclotomic polynomial sequence
 *          Phi(3, k) (or Phi(6, k)) in the order in which they occur.
 * https://oeis.org/A256148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 196, u32 n <= 733, u64 n <= 733,
 *            u128 n <= 733, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by scanning Φ(3,k)=k²+k+1 and Φ(6,k)=k²-k+1
   and collecting the first occurrence of each new primitive prime
   factor in the order of appearance.  The function memoises results
   so that calls with arbitrary n are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* ---------- static state ---------- */
static seq_t *a_seq   = NULL;   /* array of sequence values */
static size_t a_len   = 0;
static size_t a_cap   = 0;

static uint64_t *seen   = NULL; /* array of seen prime factors */
static size_t seen_len = 0;
static size_t seen_cap = 0;

static idx_t next_k = 1;        /* next k to process */

/* Ensure capacity for seq_t array.  The cast to seq_t* is needed
   for C++ compilation as realloc returns void*. */
static void ensure_seq_cap(size_t need)
{
    if (need <= a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    a_seq = (seq_t*)realloc(a_seq, new_cap * sizeof(seq_t));
    a_cap = new_cap;
}

/* Ensure capacity for seen (uint64_t) array. */
static void ensure_seen_cap(size_t need)
{
    if (need <= seen_cap) return;
    size_t new_cap = seen_cap ? seen_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seen = (uint64_t*)realloc(seen, new_cap * sizeof(uint64_t));
    seen_cap = new_cap;
}

/* Linear search – true if p already encountered. */
static bool prime_already_seen(uint64_t p)
{
    for (size_t i = 0; i < seen_len; ++i)
        if (seen[i] == p) return true;
    return false;
}

/* Record a new prime and store it as a sequence value. */
static void add_new_prime(uint64_t p)
{
    ensure_seen_cap(seen_len + 1);
    seen[seen_len++] = p;

    ensure_seq_cap(a_len + 1);
    a_seq[a_len++] = (seq_t)p;
}

/* Factor a 64‑bit integer using the supplied utility and
   record each distinct prime factor. */
static void factor_and_record(uint64_t n)
{
    if (n <= 1) return;
    uint64_t pr[20];
    uint64_t ex[20];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        if (!prime_already_seen(p))
            add_new_prime(p);
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    /* 1‑based index, produce exactly the n‑th term */
    while (a_len < n) {
        uint64_t k = (uint64_t)next_k++;

        /* Φ(3,k) = k² + k + 1 */
        uint64_t phi3 = k * k + k + 1;
        factor_and_record(phi3);

        /* Φ(6,k) = k² - k + 1 (k>=2 gives >1) */
        uint64_t phi6 = (k >= 2) ? k * k - k + 1 : 1;
        factor_and_record(phi6);
    }
    return a_seq[n - 1];
}
