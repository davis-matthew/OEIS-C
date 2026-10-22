/*
 * A356851  a(1) = 1, a(2) = 2, a(3) = 4; for n > 3, a(n) is the smallest
 *          positive number not occurring earlier such that a(n) shares a
 *          factor with the previous Omega(a(n)) terms.
 * https://oeis.org/A356851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=4; For n>3, a(n) is the smallest positive
 *            integer not used earlier that shares a factor >1 with each of
 *            the previous Ω(a(n)) terms. Ω(m) is the total number of prime
 *            factors of m counted with multiplicity
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   gcd, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/seq_realloc.h"

/* ----------------------------------------------------------------- */
/* static state                                                    */
static seq_t *seq      = NULL;      /* array of generated terms          */
static size_t seq_cap  = 0;         /* allocated capacity of seq         */
static size_t seq_len  = 0;         /* number of terms generated         */

static bool *used      = NULL;     /* bitmap of numbers already used    */
static size_t used_cap = 0;         /* maximum index covered by used     */

static uint64_t *spf    = NULL;    /* smallest prime factor table       */
static size_t     spf_cap = 0;      /* maximum index covered by spf      */

/* ----------------------------------------------------------------- */
/* helper prototypes                                               */
static void ensure_seq_cap(size_t need);
static void ensure_used_cap(size_t limit);
static void recompute_spf(size_t limit);
static uint64_t omega_uint64(uint64_t x);
static void init_sequence(void);

/* ----------------------------------------------------------------- */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t newcap = seq_cap > 0 ? seq_cap * 2 : 16;
    while (newcap < need) newcap <<= 1;
    seq = seq_realloc(seq, seq_cap, newcap);
    if (!seq) exit(1);
    seq_cap = newcap;
}

static void ensure_used_cap(size_t limit)
{
    if (limit <= used_cap) return;
    size_t newcap = used_cap > 0 ? used_cap * 2 : 256;
    while (newcap < limit) newcap <<= 1;
    bool *tmp = (bool *)realloc(used, (newcap + 1) * sizeof(bool));
    if (!tmp) exit(1);
    /* zero the new part */
    memset(tmp + used_cap + 1, 0, (newcap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = newcap;
}

static void recompute_spf(size_t limit)
{
    uint64_t *newspf = (uint64_t *)calloc(limit + 1, sizeof(uint64_t));
    if (!newspf) exit(1);
    uint64_t *pr = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    if (!pr) { free(newspf); exit(1); }
    size_t pcnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!newspf[i]) {
            newspf[i] = (uint64_t)i;
            pr[pcnt++] = (uint64_t)i;
        }
        for (size_t j = 0; j < pcnt && pr[j] <= newspf[i] && i * pr[j] <= limit; ++j)
            newspf[i * pr[j]] = pr[j];
    }
    free(pr);
    free(spf);
    spf = newspf;
    spf_cap = limit;
}

static uint64_t omega_uint64(uint64_t x)
{
    if (x <= 1) return 0;
    if (x <= spf_cap) {
        uint64_t cnt = 0;
        while (x > 1) {
            uint64_t p = spf[x];
            do { x /= p; ++cnt; } while (x > 1 && x % p == 0);
        }
        return cnt;
    }
    uint64_t cnt = 0;
    for (uint64_t d = 2; d * d <= x; ++d) {
        if (x % d == 0) {
            do { x /= d; ++cnt; } while (x % d == 0);
        }
    }
    if (x > 1) ++cnt;
    return cnt;
}

static void init_sequence(void)
{
    /* seed first three terms */
    ensure_seq_cap(3);
    seq[0] = (seq_t)1;
    seq[1] = (seq_t)2;
    seq[2] = (seq_t)4;
    seq_len = 3;

    /* mark used numbers */
    ensure_used_cap(4);
    used[1] = true;
    used[2] = true;
    used[4] = true;

    /* initial SPF table */
    recompute_spf(4);
}

/* ----------------------------------------------------------------- */
static bool sequence_initialized = false;

seq_t a(idx_t n)
{
    if (!sequence_initialized) {
        init_sequence();
        sequence_initialized = true;
    }

    if (n == 1) return seq[0];
    if (n == 2) return seq[1];
    if (n == 3) return seq[2];

    /* generate terms up to n */
    while ((idx_t)seq_len < n) {
        /* find smallest unused candidate satisfying the rule */
        uint64_t cand = 2;
        for (;;) {
            ++cand;

            /* skip used numbers */
            if (cand <= used_cap) {
                if (used[cand]) continue;
            } else {
                ensure_used_cap(cand);
                if (used[cand]) continue;  /* should not happen, but be safe */
            }

            /* extend SPF table if needed */
            if (cand > spf_cap) recompute_spf((size_t)cand);

            uint64_t omega = omega_uint64(cand);
            if (omega > seq_len) continue; /* not enough previous terms */

            bool ok = true;
            for (uint64_t i = 0; i < omega; ++i) {
                seq_t g = gcd((seq_t)cand, seq[seq_len - 1 - i]);
                if (g <= (seq_t)1) { ok = false; break; }
            }
            if (!ok) continue;

            /* accept cand */
            used[cand] = true;
            ensure_seq_cap(seq_len + 1);
            seq[seq_len] = (seq_t)cand;
            ++seq_len;
            break;
        }
    }
    return seq[n - 1];
}
