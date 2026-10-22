/*
 * A381098  Irregular triangle read by rows: row 1 = (1, 2); row n+1 has
 *          length L = last element of row n, and consists of the L smallest
 *          positive integers not occurring earlier that share a factor with
 *          L.
 * https://oeis.org/A381098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * The function computes OEIS sequence A381098.
 * A(1)=1, A(2)=2; for n>2, A is built row by row as described
 * in the OEIS entry using the constructive algorithm.
 * The implementation memoises the sequence in static arrays.
 */
 
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *seq_cache = NULL;          /* memoised terms */
static idx_t seq_len   = 0;              /* number of terms currently cached */
static idx_t seq_cap   = 0;              /* allocated capacity of seq_cache */

static uint8_t *used_bitmap = NULL;      /* 1‑byte per integer used flag */
static idx_t used_cap   = 0;             /* allocated size of used_bitmap */

/* Ensure seq_cache has capacity 'needed' entries */
static void ensure_seq_capacity(idx_t needed)
{
    if (needed <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap << 1 : 128;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *newptr = (seq_t *)realloc(seq_cache, (size_t)new_cap * sizeof(seq_t));
    if (!newptr) abort();
    if (new_cap > seq_cap)
        memset(newptr + seq_cap, 0, (size_t)(new_cap - seq_cap) * sizeof(seq_t));
    seq_cache = newptr;
    seq_cap   = new_cap;
}

/* Ensure used_bitmap can index up to 'idx' (inclusive) */
static void ensure_used_capacity(idx_t idx)
{
    if (idx < used_cap) return;
    idx_t new_cap = used_cap ? used_cap << 1 : 128;
    while (new_cap <= idx) new_cap <<= 1;
    uint8_t *newptr = (uint8_t *)realloc(used_bitmap, (size_t)new_cap);
    if (!newptr) abort();
    if (new_cap > used_cap)
        memset(newptr + used_cap, 0, (size_t)(new_cap - used_cap));
    used_bitmap = newptr;
    used_cap    = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* initialisation on first call */
    if (seq_len == 0) {
        ensure_seq_capacity(64);
        seq_cache[0] = 1;
        seq_cache[1] = 2;
        seq_len = 2;

        ensure_used_capacity(2);
        used_bitmap[1] = 1;
        used_bitmap[2] = 1;
    }

    /* generate rows until we have at least n terms */
    while (seq_len < n) {
        /* length of next row = last element of previous row */
        seq_t last_val   = seq_cache[seq_len - 1];
        idx_t row_len    = (idx_t)last_val;     /* last_val fits in idx_t */

        /* factorise row_len into distinct primes (stored in primes[]) */
        idx_t primes[64];
        size_t pcnt = 0;
        idx_t tmp = row_len;
        for (idx_t p = 2; p <= tmp / p; ++p) {
            if (tmp % p == 0) {
                primes[pcnt++] = p;
                while (tmp % p == 0) tmp /= p;
            }
        }
        if (tmp > 1 && pcnt < 64) primes[pcnt++] = tmp;

        /* for each prime factor keep the next unused multiple */
        idx_t next_mul[64];
        for (size_t i = 0; i < pcnt; ++i) {
            idx_t p = primes[i];
            idx_t val = p;
            while (true) {
                if (val >= used_cap) ensure_used_capacity(val);
                if (!used_bitmap[val]) break;
                val += p;
            }
            next_mul[i] = val;
        }

        /* produce row_len elements */
        for (idx_t k = 0; k < row_len; ++k) {
            /* smallest available candidate among all primes' multiples */
            idx_t cand = next_mul[0];
            for (size_t i = 1; i < pcnt; ++i)
                if (next_mul[i] < cand) cand = next_mul[i];

            /* append candidate */
            ensure_seq_capacity(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)cand;

            /* mark as used */
            if (cand >= used_cap) ensure_used_capacity(cand);
            used_bitmap[cand] = 1;

            /* advance all primes whose multiple equals the chosen candidate */
            for (size_t i = 0; i < pcnt; ++i) {
                if (next_mul[i] == cand) {
                    idx_t p = primes[i];
                    idx_t v = cand + p;
                    while (true) {
                        if (v >= used_cap) ensure_used_capacity(v);
                        if (!used_bitmap[v]) break;
                        v += p;
                    }
                    next_mul[i] = v;
                }
            }
        }
    }

    return seq_cache[n - 1];
}
