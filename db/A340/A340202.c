/*
 * A340202  Primes followed by the difference from the next prime calculated
 *          by adding the digits of the lexicographically earliest distinct
 *          term.
 * https://oeis.org/A340202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A340202
   a(1) = 2
   For k ≥ 1:
     a(2k-1) = k‑th prime (2,3,5,7,…)
     a(2k)   = the lexicographically earliest unused non‑prime
               whose decimal digits sum to (p_k+1 − p_k),
               where p_k is the k‑th prime.
   The function is memoised and works for any n ≥ 1 in any call order. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../utils/build_primes_upto.h"   /* build_primes_upto */
#include "../utils/is_prime_big.h"        /* is_prime_big */

/* ------------------------------------------------------------ */
/* Global working state                                          */
/* ------------------------------------------------------------ */
static seq_t *seq_cache   = NULL;   /* cached a(i) terms          */
static size_t seq_size    = 0;        /* size of the cache          */

static uint64_t *primes   = NULL;    /* list of primes             */
static size_t   primes_cnt = 0;       /* number of primes           */

static seq_t *used       = NULL;     /* array of already used terms*/
static size_t used_cnt   = 0;        /* used count                 */
static size_t used_cap   = 0;        /* used array capacity        */

/* ------------------------------------------------------------ */
/* Utility helpers for the working state                         */
/* ------------------------------------------------------------ */
static void ensure_used_capacity(size_t needed)
{
    if (needed <= used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(used, new_cap * sizeof(seq_t));
    used = tmp;
    used_cap = new_cap;
}

static void used_push(seq_t v)
{
    ensure_used_capacity(used_cnt + 1);
    used[used_cnt++] = v;
}

static bool used_exists(seq_t v)
{
    for (size_t i = 0; i < used_cnt; ++i)
        if (used[i] == v) return true;
    return false;
}

/* ------------------------------------------------------------ */
/* Candidate generation: smallest unused non‑prime with digit sum S */
/* ------------------------------------------------------------ */
static bool rec_candidate(int pos, int len, seq_t val, int rem, seq_t *out)
{
    if (pos == len) {
        /* all digits chosen */
        if (rem != 0) return false;
        if (used_exists(val) || is_prime_big(val)) return false;
        *out = val;
        return true;
    }

    int start = (pos == 0) ? 1 : 0;
    for (int d = start; d <= 9 && d <= rem; ++d) {
        /* remaining digits after choosing d must be able to sum to rem-d */
        int max_possible = 9 * (len - pos - 1);
        if (rem - d > max_possible) continue;
        if (rec_candidate(pos + 1, len, val * 10 + d, rem - d, out))
            return true;
    }
    return false;
}

static seq_t find_candidate(int digit_sum)
{
    for (int len = 1; ; ++len) {
        seq_t val;
        if (rec_candidate(0, len, 0, digit_sum, &val))
            return val;
    }
}

/* ------------------------------------------------------------ */
/* Prime generation (once)                                       */
/* ------------------------------------------------------------ */
static void init_primes(void)
{
    if (primes) return;
    /* Enough primes for n up to 10000 – about 200000 yields several
       thousand primes, sufficient for the required differences. */
    idx_t limit = 200000;
    build_primes_upto(limit, &primes, &primes_cnt);
}

/* ------------------------------------------------------------ */
/* Main sequence function                                        */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1 */

    init_primes();

    /* Grow cache if necessary */
    if (n > seq_size) {
        size_t old = seq_size;
        seq_size = n;
        if (!seq_cache)
            seq_cache = (seq_t *)malloc(seq_size * sizeof(seq_t));
        else
            seq_cache = (seq_t *)realloc(seq_cache, seq_size * sizeof(seq_t));

        for (size_t i = old; i < seq_size; ++i) {
            size_t idx = i + 1;                /* 1‑based */
            if (idx & 1) {                     /* odd index → prime */
                size_t pidx = (idx - 1) >> 1;
                seq_t p = primes[pidx];
                seq_cache[i] = p;
                used_push(p);                  /* mark used */
            } else {                           /* even index → candidate */
                size_t k = (idx - 1) >> 1;      /* 0‑based index of prime pair */
                seq_t cur  = primes[k];
                seq_t next = primes[k + 1];
                int diff = (int)(next - cur);  /* diff is small and non‑negative */
                seq_t cand = find_candidate(diff);
                seq_cache[i] = cand;
                used_push(cand);                /* mark used */
            }
        }
    }
    return seq_cache[n - 1];
}
