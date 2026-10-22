/*
 * A399864  Numbers k in A024619 such that A376846(k) = 0.
 * https://oeis.org/A399864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 4869, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A399864:
   Numbers k with at least two distinct prime factors for which
   for every m ≤ k with rad(m) | k, Omega(m) ≤ Omega(k). */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define LIMIT 5000000     /* enough for n up to ~10 000 terms */

static idx_t *spf = NULL;          /* smallest prime factor for each n */
static uint8_t *best_omega = NULL; /* best Omega seen so far for each radical */
static seq_t *seq_vals = NULL;     /* cached sequence terms */
static size_t seq_len  = 0;         /* number of terms stored */
static size_t seq_cap  = 0;         /* allocated capacity of seq_vals */
static idx_t  current_m = 2;       /* last integer processed in the sieve */

static void grow_seq(size_t new_len)
{
    if (new_len <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 32;
    while (new_cap < new_len) new_cap <<= 1;
    seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!seq_vals) abort();
    seq_cap = new_cap;
}

static void init_sieve(void)
{
    if (spf != NULL) return;            /* already initialized */

    spf = (idx_t *)calloc((size_t)(LIMIT + 1), sizeof(idx_t));
    if (!spf) abort();

    best_omega = (uint8_t *)calloc((size_t)(LIMIT + 1), sizeof(uint8_t));
    if (!best_omega) abort();

    /* linear sieve for spf */
    idx_t *primes = (idx_t *)malloc((LIMIT / 10) * sizeof(idx_t));
    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (primes_cnt < LIMIT / 10) primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes[j];
            if (p > spf[i] || i * p > LIMIT) break;
            spf[i * p] = p;
        }
    }
    free(primes);
}

static void generate_up_to(idx_t needed)
{
    init_sieve();

    while (seq_len < needed) {
        if (current_m > LIMIT) abort(); /* limit exceeded */

        idx_t n = current_m;
        idx_t last_p = 0;
        int omega = 0;
        int dcnt = 0;
        idx_t rad = 1;
        idx_t distinct[16]; /* sufficient for LIMIT */

        /* factor n using spf */
        while (n > 1) {
            idx_t p = spf[n];
            ++omega;
            if (p != last_p) {
                distinct[dcnt++] = p;
                rad *= p;
                last_p = p;
            }
            n /= p;
        }

        bool good = true;
        if (dcnt >= 2) {
            int total = 1 << dcnt;
            for (int mask = 0; mask < total && good; ++mask) {
                idx_t d = 1;
                for (int i = 0; i < dcnt; ++i)
                    if (mask & (1 << i))
                        d *= distinct[i];
                if (best_omega[d] > omega)
                    good = false;
            }
            if (good) {
                grow_seq(seq_len + 1);
                seq_vals[seq_len++] = (seq_t)current_m;
            }
        }

        /* update best_omega regardless of goodness */
        if (rad <= LIMIT && (uint8_t)omega > best_omega[rad])
            best_omega[rad] = (uint8_t)omega;

        ++current_m;
    }
}

/* Public function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* sequence is 1‑indexed */
    generate_up_to(n);
    return seq_vals[n - 1];
}
