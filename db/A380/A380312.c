/*
 * A380312  Primes not reaching 3 under iterations of p -> gpf(2*p-1).
 * https://oeis.org/A380312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   gpf, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A380312: primes not reaching 3 under iterations of
   p -> gpf(2*p-1).  The function enumerates primes in ascending order,
   follows the iteration until it enters either the 3‑cycle (3,5) or the
   19‑cycle (19,37,73,29); primes that reach the 19‑cycle are stored,
   and a(n) returns the n‑th stored prime (offset 1).  The routine
   memoises the outcome of each discovered prime to keep the work
   essentially linear. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/gpf.h"
#include "../utils/next_prime.h"

/* ----------  memoisation status for primes  ---------- */
static uint8_t *status = NULL;   /* 0 = unknown, 1 = leads to 19, 2 = leads to 3, 3 = on stack */
static size_t status_cap = 0;

static void ensure_status_cap(idx_t n)
{
    if ((idx_t)status_cap > n) return;
    size_t new_cap = status_cap ? status_cap << 1 : 128;
    while ((idx_t)new_cap <= n) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(status, new_cap);
    if (!tmp) abort();
    /* initialise new section to zero */
    memset(tmp + status_cap, 0, new_cap - status_cap);
    status = tmp;
    status_cap = new_cap;
}

/* does the prime p eventually reach the 19‑cycle? */
static bool leads_to_19(uint64_t p)
{
    if (p < 2) return false;
    ensure_status_cap(p);
    uint8_t s = status[p];
    if (s == 1) return true;
    if (s == 2) return false;
    if (s == 3) {          /* cycle detected; treat as failure */
        status[p] = 2;
        return false;
    }
    status[p] = 3;         /* temporarily mark */
    uint64_t next = gpf(2 * p - 1);
    bool res = leads_to_19(next);
    status[p] = res ? 1 : 2;
    return res;
}

/* ----------  dynamic array for the sequence  ---------- */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint64_t last_prime = 0;   /* last prime examined */

static void grow_seq_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = *cap ? *cap << 1 : 4;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    *arr = tmp;
    *cap = new_cap;
}

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (seq_len < (size_t)n) {
        /* initialise status for small primes once */
        if (!status) {
            ensure_status_cap(20);
            /* 19‑cycle */
            status[19] = 1;
            status[37] = 1;
            status[73] = 1;
            status[29] = 1;
            /* 3‑cycle (3,5) */
            status[3]  = 2;
            status[5]  = 2;
        }

        for (; seq_len < (size_t)n; ) {
            last_prime = next_prime(last_prime);
            if (leads_to_19(last_prime)) {
                grow_seq_array(&seq_cache, &seq_cap, seq_len + 1);
                seq_cache[seq_len++] = (seq_t)last_prime;
            }
        }
    }
    return seq_cache[n - 1];
}
