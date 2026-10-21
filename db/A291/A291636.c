/*
 * A291636  Matula-Goebel numbers of lone-child-avoiding rooted trees.
 * https://oeis.org/A291636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th smallest positive integer that is either 1 or a
 *            composite number whose prime factors have indices that are
 *            themselves in the sequence, counted with multiplicity. It is
 *            computed by an incremental sieve that checks each integer in
 *            natural order
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static idx_t sieve_limit = 0;          /* current maximum of the sieve   */
static uint32_t *spf = NULL;           /* smallest prime factor table    */
static uint32_t *prime_idx = NULL;     /* prime index mapping (prime->idx) */
static uint8_t  *in_seq = NULL;        /* 0/1 whether a number is in seq */

static idx_t seq_cap = 0;              /* capacity of seq array          */
static seq_t *seq_arr = NULL;          /* sequence values                */
static idx_t seq_len = 0;              /* number of terms stored         */

static idx_t current_checked = 0;      /* highest integer already processed */

/* Recompute the sieve up to 'limit', preserving known 'in_seq' flags. */
static void recompute_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;          /* nothing to do             */

    uint32_t *new_spf = NULL;
    uint32_t *new_prime_idx = NULL;
    uint8_t  *new_in_seq = NULL;

    /* Allocate new tables. */
    new_spf = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
    new_prime_idx = (uint32_t *)realloc(prime_idx, (limit + 1) * sizeof(uint32_t));
    new_in_seq = (uint8_t *)realloc(in_seq, (limit + 1) * sizeof(uint8_t));
    if (!new_spf || !new_prime_idx || !new_in_seq) {
        /* Allocation failure; abort. */
        exit(EXIT_FAILURE);
    }

    /* Copy previous flags to preserve sequence information. */
    if (sieve_limit > 0) {
        memcpy(new_in_seq, in_seq, (sieve_limit + 1) * sizeof(uint8_t));
    }

    spf = new_spf;
    prime_idx = new_prime_idx;
    in_seq = new_in_seq;

    /* Linear sieve for spf and prime indices. */
    uint32_t *primes = NULL;
    size_t primes_cap = 16;
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    size_t primes_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = (uint32_t)i;
            prime_idx[i] = (uint32_t)(primes_cnt);  /* 1‑based index */
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            idx_t prod = i * (idx_t)p;
            if (prod > limit) break;
            spf[prod] = p;
            if (p > spf[i]) break;
        }
    }
    free(primes);

    /* Ensure 1 is marked as in sequence. */
    if (!in_seq) in_seq = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    memset(in_seq, 0, (limit + 1) * sizeof(uint8_t));
    if (limit >= 1) in_seq[1] = 1;

    sieve_limit = limit;
}

/* Ensure the sequence array has space for at least 'needed' elements. */
static void ensure_seq_capacity(idx_t needed)
{
    if (seq_cap >= needed) return;
    idx_t new_cap = seq_cap ? seq_cap << 1 : 64;
    if (new_cap < needed) new_cap = needed;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Main function: return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if ((n == 0) || (n > UINT64_MAX)) /* undefined for n < 1; guard */ {
        return (seq_t)0;
    }

    /* First call: initialise data structures. */
    if (seq_cap == 0) {
        seq_cap = 64;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
        seq_arr[seq_len++] = (seq_t)1;   /* a(1) = 1 */
        recompute_sieve(1024);            /* initial sieve */
        current_checked = 1;
    }

    /* Generate terms until we have a(n). */
    while (seq_len < n) {
        ++current_checked;
        if (current_checked > sieve_limit) {
            /* Extend sieve when exceeding current limit. */
            recompute_sieve(current_checked * 2 + 100);
        }

        /* Factor current_checked using spf. */
        idx_t temp = current_checked;
        idx_t prev_idx = 0;
        int distinct = 0;
        int exp = 0;
        bool ok = true;

        while (temp > 1) {
            uint32_t p = spf[temp];
            idx_t idxp = prime_idx[p];
            /* p is guaranteed to be prime, so idxp > 0. */
            if (idxp != prev_idx) {
                if (distinct > 0) { /* new distinct prime */
                    if (!in_seq[idxp]) { ok = false; break; }
                } else { /* first prime factor */
                    if (!in_seq[idxp]) { ok = false; break; }
                }
                distinct++;
                prev_idx = idxp;
                exp = 1;
            } else {
                exp++;
            }
            temp /= p;
        }

        if (ok && !(distinct == 1 && exp < 2)) {
            /* Accept the number. */
            in_seq[current_checked] = 1;
            ensure_seq_capacity(seq_len + 1);
            seq_arr[seq_len++] = (seq_t)current_checked;
        }
    }

    return seq_arr[n - 1];
}
