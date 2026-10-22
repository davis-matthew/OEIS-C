/*
 * A306998  List of low point records for pi(n)/n.
 * https://oeis.org/A306998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 7874, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A306998: list of n such that pi(n)/n is a new record low. */
/* The value a(n) is the n‑th such record.  The implementation builds
   a sieve of Eratosthenes and tracks pi(n) incrementally.  The function
   works for any n ≥ 1.  Calls may come in arbitrary order; internally
   the sequence is memoized so higher indices can be reused. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Types defined by the build system */
typedef idx_t idx_t;
typedef seq_t seq_t;

/* ---------- static state ---------------------------------------- */
static idx_t sieve_limit = 0;          /* current sieve size        */
static uint8_t *sieve = NULL;          /* composite flag array      */

static idx_t current_n = 1;            /* last number processed     */
static idx_t pi_count  = 0;            /* pi(current_n)             */

static idx_t best_n = 0;               /* n giving current lowest ratio */
static idx_t best_pi = 0;              /* pi(best_n)                 */

static idx_t *records = NULL;          /* array of record n values   */
static size_t   rec_len = 0;           /* number of records stored   */
static size_t   rec_cap = 0;           /* capacity of records array  */

/* ---------- helpers ------------------------------------------------ */

static void record_add(idx_t n)
{
    if (rec_cap == 0) {
        rec_cap = 16;
        records = (idx_t *)malloc(rec_cap * sizeof(idx_t));
    } else if (rec_len >= rec_cap) {
        rec_cap <<= 1;
        records = (idx_t *)realloc(records, rec_cap * sizeof(idx_t));
    }
    records[rec_len++] = n;
}

static void rebuild_sieve(idx_t new_limit)
{
    uint8_t *tmp = (uint8_t *)realloc(sieve, new_limit + 1);
    if (!tmp) exit(1);             /* allocation failure */
    sieve = tmp;
    memset(sieve, 0, new_limit + 1);

    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i) {
                sieve[j] = 1;
            }
        }
    }
    sieve_limit = new_limit;
}

static void ensure_records(idx_t target_index)
{
    if (rec_len >= target_index) return;

    if (sieve_limit == 0) {
        sieve_limit = 8192;
        rebuild_sieve(sieve_limit);
    }

    while (rec_len < target_index) {
        /* extend the sieve if required */
        if (current_n + 1 > sieve_limit) {
            idx_t new_limit = sieve_limit * 2;
            if (new_limit == 0) new_limit = 8192;
            rebuild_sieve(new_limit);
        }

        ++current_n;
        if (current_n >= 2 && !sieve[current_n]) ++pi_count;

        if (rec_len == 0) {               /* first record – n=2 */
            if (current_n == 2) {
                record_add(current_n);
                best_n   = current_n;
                best_pi  = pi_count;
            }
            continue;
        }

        /* Compare pi(current_n)/current_n with current best ratio */
        unsigned __int128 left  = (unsigned __int128)pi_count * best_n;
        unsigned __int128 right = (unsigned __int128)best_pi  * current_n;
        if (left < right) {
            record_add(current_n);
            best_n  = current_n;
            best_pi = pi_count;
        }
    }
}

/* ---------- public sequence function ----------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;         /* undefined for n < 1 */
    ensure_records(n);
    return (seq_t)records[n - 1];
}
