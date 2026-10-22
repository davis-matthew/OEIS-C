/*
 * A319705  Filter sequence which for primes p records a distinct value for
 *          each distinct multiset formed from the lengths of 1-runs in its
 *          binary representation [A286622(p)], and for all other numbers
 *          assigns a unique number.
 * https://oeis.org/A319705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 298, u16 n <= 9255, u32 n <= 9255, u64 n <= 9255,
 *            u128 n <= 9255, big any n
 * Helpers:   is_prime, key_hash, prime_run_lengths
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Restricted growth sequence transform of the function  
   f(n) = A278222(n) when n is prime, otherwise f(n) = -n. 
   The sequence a(n) is the index of the first occurrence of f(n) among
   all distinct f values, numbered from 1.
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/key_hash.h"
#include "../utils/prime_run_lengths.h"

#define KEY_CAP 8192   /* power‑of‑two table size (enough for <~2000 keys) */

typedef struct {
    uint8_t runs[64];   /* sorted run lengths of 1’s (max 64 bits) */
    size_t  len;        /* number of runs stored */
    seq_t   label;      /* label assigned to this key (first prime that produced it) */
    bool    used;       /* flag whether slot is occupied */
} KeyEntry;

static seq_t *seq_arr      = NULL;  /* a[1..max], 1‑based */
static size_t seq_cap      = 0;
static idx_t  seq_max      = 0;      /* highest n computed so far */
static seq_t  next_label   = 1;      /* next fresh label to assign */

static KeyEntry *key_tbl   = NULL;   /* hash table of {run lengths → label} */

/* --- Lookup or insert key in hash table ------------------------------ */
static seq_t key_find_or_insert(const uint8_t *runs, size_t len)
{
    uint64_t h = key_hash(runs, len);
    size_t idx = h & (KEY_CAP - 1);

    for (;;) {
        KeyEntry *e = &key_tbl[idx];
        if (!e->used) {
            /* new key */
            e->used = true;
            e->len  = len;
            memcpy(e->runs, runs, len);
            e->label = next_label;
            next_label++;
            return e->label;
        }
        if (e->len == len && memcmp(e->runs, runs, len) == 0) {
            return e->label;          /* existing key */
        }
        idx = (idx + 1) & (KEY_CAP - 1);
    }
}

/* --- Ensure that a[1..n] is computed ---------------------------------- */
static void extend_to(idx_t n)
{
    if (n <= seq_max) return;

    /* grow the a[] array if needed */
    if (n + 1 > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap : 64;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* initialise key table once */
    if (!key_tbl) {
        key_tbl = (KeyEntry *)calloc(KEY_CAP, sizeof(KeyEntry));
    }

    /* compute values from seq_max+1 up to n */
    uint8_t runs[64];
    size_t  run_len;

    for (idx_t i = seq_max + 1; i <= n; ++i) {
        if (is_prime(i)) {
            prime_run_lengths(i, runs, &run_len);
            seq_arr[i] = key_find_or_insert(runs, run_len);
        } else {
            seq_arr[i] = next_label;
            next_label++;
        }
    }

    seq_max = n;
}

/* ----- public function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;      /* undefined, return 0 */
    extend_to(n);
    return seq_arr[n];
}
