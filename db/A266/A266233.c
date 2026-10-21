/*
 * A266233  Primes representable as f(f(f(...f(p)...))) where p is a prime and
 *          f(x) = x*2 + 1.
 * https://oeis.org/A266233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence consists of all prime numbers that can be written as
 *  f(f(⋯f(p)⋯)) for some prime p, where f(x) = 2*x + 1.
 *  The value a(n) is the n-th element of this set, indexed from 1.
 *  The implementation builds a sieve of primes up to a dynamic
 *  limit and stores all primes that satisfy the condition.
 *  It is fully thread‑safe only for independent calls; the static
 *  data is updated lazily on demand.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Function prototype expected by the build system */
seq_t a(idx_t n);

/* ---------- static data for incremental sieve and sequence ---------- */
static size_t sieve_limit = 0;             /* current sieve upper bound            */
static uint8_t *sieve_primality = NULL;    /* 1 => prime, 0 => composite          */
static uint64_t *prime_list = NULL;        /* primes computed for current sieve    */
static size_t prime_count = 0;             /* number of primes in prime_list       */

static seq_t *seq_list = NULL;             /* sequence values that satisfy rule    */
static size_t seq_len = 0;                 /* number of values in seq_list         */

/* ---------- helper: rebuild the sieve and recompute the sequence ---------- */
static void rebuild_sieve(size_t new_limit)
{
    /*--- allocate primality array ---*/
    uint8_t *new_sieve = (uint8_t *)malloc(new_limit + 1);
    if (!new_sieve) return;               /* allocation failed – abort        */
    memset(new_sieve, 1, new_limit + 1);
    new_sieve[0] = new_sieve[1] = 0;

    /*--- Eratosthenes sieve ---*/
    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (new_sieve[i]) {
            for (size_t j = i * i; j <= new_limit; j += i)
                new_sieve[j] = 0;
        }
    }

    /*--- build list of primes up to new_limit ---*/
    size_t cnt = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (new_sieve[i]) ++cnt;

    uint64_t *new_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_primes) {
        free(new_sieve);
        return;
    }
    size_t id = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (new_sieve[i]) new_primes[id++] = i;

    /*--- recompute the sequence ---*/
    size_t seq_capacity = cnt;           /* worst case: all primes qualify     */
    seq_t *new_seq = (seq_t *)malloc(seq_capacity * sizeof(seq_t));
    if (!new_seq) {
        free(new_sieve);
        free(new_primes);
        return;
    }
    size_t seq_id = 0;

    for (size_t pi = 0; pi < cnt; ++pi) {
        uint64_t p = new_primes[pi];
        if (p <= 2) continue;              /* need k >= 1, so 2 itself is not output */

        uint64_t t = (p - 1) / 2;
        bool ok = false;
        while (t >= 2) {
            if (t <= new_limit && new_sieve[t]) { ok = true; break; }
            if ((t & 1) == 0) break;        /* even > 2 cannot continue the chain */
            t = (t - 1) / 2;
        }
        if (ok) {
            if (seq_id == seq_capacity) { /* unlikely, but guard against overflow */
                seq_capacity *= 2;
                seq_t *tmp = (seq_t *)realloc(new_seq,
                                              seq_capacity * sizeof(seq_t));
                if (!tmp) {
                    free(new_sieve);
                    free(new_primes);
                    free(new_seq);
                    return;
                }
                new_seq = tmp;
            }
            new_seq[seq_id++] = (seq_t)p;
        }
    }

    /*--- replace old data with the new ones ---*/
    free(sieve_primality);
    free(prime_list);
    free(seq_list);
    sieve_primality = new_sieve;
    prime_list      = new_primes;
    prime_count     = cnt;
    seq_list        = new_seq;
    seq_len         = seq_id;
    sieve_limit     = new_limit;
}

/* ---------- main sequence accessor ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 – return 0      */

    /*--- ensure we have enough terms by expanding the sieve if needed ---*/
    while (seq_len < (size_t)n) {
        size_t new_limit = sieve_limit ? sieve_limit * 2 : 100000;
        rebuild_sieve(new_limit);
        if (sieve_limit == 0)                /* first call failed (unlikely) */
            return (seq_t)0;
    }

    return seq_list[n - 1];
}
