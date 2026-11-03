/*
 * A325238  First positive integer with each omega-sequence.
 * https://oeis.org/A325238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   bigomega, factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the first positive integer with each omega‑sequence (OEIS A325238)
 * by exhaustive search.  The function returns the n‑th value a(n), where n≥1. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <string.h>
#include "../utils/bigomega.h"
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

typedef struct {
    size_t len;          /* length of omega‑sequence */
    idx_t *data;         /* array of Omega values */
    seq_t value;         /* minimal integer having this sequence */
} OmegaSeq;

/* --- static storage ----------------------------------------------------- */
static OmegaSeq *seqs   = NULL;   /* array of discovered sequences       */
static size_t seqs_size = 0;      /* capacity of seqs array              */
static size_t seqs_cnt  = 0;      /* number of sequences stored          */
static idx_t  processed = 0;      /* last integer processed while generating sequences */

/* --- helper functions --------------------------------------------------- */
static bool seq_exists(const OmegaSeq *s)
{
    for (size_t i = 0; i < seqs_cnt; ++i) {
        if (seqs[i].len != s->len)
            continue;
        bool equal = true;
        for (size_t j = 0; j < s->len; ++j) {
            if (seqs[i].data[j] != s->data[j]) {
                equal = false;
                break;
            }
        }
        if (equal)
            return true;
    }
    return false;
}

/* Compute the omega‑sequence of n and store it in seq.
 * seq->value is set to n. */
static void compute_omega_sequence(idx_t n, OmegaSeq *seq)
{
    if (n == 1) {                /* empty sequence for 1 */
        seq->len   = 0;
        seq->data  = NULL;
        seq->value = (seq_t)n;
        return;
    }

    /* Temporary buffer for Omega values (length <= 64 is sufficient). */
    idx_t buffer[64];
    size_t len = 0;

    seq_t cur = (seq_t)n;

    while (1) {
        /* Get Omega of current integer. */
        seq_t omega_seqt = bigomega((idx_t)cur);
        idx_t omega = (idx_t)omega_seqt;
        buffer[len++] = omega;

        if (omega == 1)
            break;

        /* Factor cur to obtain exponents. */
        uint64_t primes_buf[64];
        uint64_t exps_buf[64];
        size_t num_factors;
        factor_u64((uint64_t)cur, primes_buf, exps_buf, &num_factors);

        /* Build next integer: product of primes corresponding to exponents. */
        seq_t next = 1;
        for (size_t i = 0; i < num_factors; ++i) {
            seq_t prime_val = (seq_t)get_prime((idx_t)exps_buf[i]);
            next = seq_mul(next, prime_val);
        }
        cur = next;
    }

    seq->len   = len;
    if (len == 0)
        seq->data = NULL;
    else
        seq->data = (idx_t *)malloc(len * sizeof(idx_t));
    if (len > 0)
        memcpy(seq->data, buffer, len * sizeof(idx_t));
    seq->value = (seq_t)n;
}

/* --- public function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n distinct omega‑sequences. */
    while (seqs_cnt < n) {
        ++processed;                       /* next integer to examine   */
        OmegaSeq candidate;
        compute_omega_sequence(processed, &candidate);

        if (!seq_exists(&candidate)) {
            /* ensure capacity for new sequence */
            if (seqs_cnt == seqs_size) {
                seqs_size = seqs_size ? seqs_size * 2 : 16;
                seqs = mem_resize(OmegaSeq, seqs, seqs_size);
            }
            seqs[seqs_cnt] = candidate;     /* ownership transferred   */
            ++seqs_cnt;
        } else {
            /* duplicate: free allocated data */
            if (candidate.data != NULL)
                free(candidate.data);
        }
    }

    return seqs[n - 1].value;
}
