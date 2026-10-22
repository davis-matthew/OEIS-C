/*
 * A351059  a(n) is the smallest number not yet in the sequence that has three
 *          digits in common with a(n-1), starting with a(1) = 1.
 * https://oeis.org/A351059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A351059:  a(1)=1.
    For n>1, a(n) is the smallest positive integer not yet appearing in the
    sequence that shares exactly three “common digits” with a(n−1),
    where the common‑digit count is defined as
        sum_{d=0..9}    count_{a(n−1)}(d) × count_{a(n)}(d).
    The sequence values are computed symbolically; the algorithm uses only
    integer arithmetic, bit manipulation and static storage.
    It is valid C11 and C++17.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ------------- digit counting helper ------------------------------------ */
static void digit_counts(seq_t x, unsigned char out[10]) {
    /* zero the table */
    memset(out, 0, 10u);
    if (x == (seq_t)0) {            /* the number 0 has one zero digit */
        out[0] = 1;
        return;
    }
    while (x != (seq_t)0) {
        unsigned int d = (unsigned int)(x % (seq_t)10);
        out[d]++;                    /* count digit d of x */
        x /= (seq_t)10;
    }
}

/* ------------- storage containers -------------------------------------- */
/* static sequence storage */
static seq_t *seq = NULL;
static size_t  seq_cap = 0;
static size_t  seq_len = 0;

/* static usage bitset, indexed by the numerical value */
static unsigned char *used_tbl = NULL;   /* 1 = used, 0 = free */
static size_t   used_cap = 0;           /* allocated length */

/* smallest unused positive integer (mex) */
static uint64_t mex = 1;                /* 1 is used after init */

/* boolean flag for static initialization */
static bool  initialized = false;

/* ------------- helper to grow the sequence array -------------------- */
static void ensure_seq_capacity(size_t needed) {
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ------------- helper to grow the usage table ----------------------- */
static void ensure_used_capacity(uint64_t up_to) {
    if (used_cap > up_to) return;
    size_t new_cap = used_cap ? used_cap : 16;
    while (new_cap <= (size_t)up_to) new_cap <<= 1;
    used_tbl = (unsigned char *)realloc(used_tbl, new_cap * sizeof(unsigned char));
    memset(used_tbl + used_cap, 0, (new_cap - used_cap) * sizeof(unsigned char));
    used_cap = new_cap;
}

/* ------------- main sequence routine ---------------------------------- */
seq_t a(idx_t n) {
    /* one‑time static setup */
    if (!initialized) {
        /* allocate initial storage */
        ensure_seq_capacity(16);
        ensure_used_capacity(1);
        /* first term */
        seq[0] = (seq_t)1;
        seq_len = 1;
        used_tbl[1] = 1;           /* mark 1 as used */
        /* mex already correctly set to 2 after the insertion above */
        initialized = true;
    }

    /* extend sequence up to required index */
    while (seq_len < (size_t)n) {
        seq_t prev = seq[seq_len - 1];
        unsigned char prev_cnt[10];
        digit_counts(prev, prev_cnt);

        /* find the smallest unused number with 3 common digits */
        uint64_t cand = 1;
        for (;;) {
            if (cand >= used_cap)   /* grow usage table if necessary */
                ensure_used_capacity(cand);
            if (!used_tbl[cand]) {   /* unused so far */
                unsigned char cand_cnt[10];
                digit_counts((seq_t)cand, cand_cnt);
                int common = 0;
                for (int d = 0; d < 10; ++d)
                    common += (int)(prev_cnt[d] * cand_cnt[d]);
                if (common == 3) {         /* satisfies the rule */
                    /* store the new term */
                    if (seq_len >= seq_cap)   /* grow sequence array */
                        ensure_seq_capacity(seq_len + 1);
                    seq[seq_len++] = (seq_t)cand;
                    used_tbl[cand] = 1;   /* mark as used */

                    /* update mex if this candidate was the current smallest unused */
                    if (cand == mex) {
                        do {
                            ++mex;
                            if (mex >= used_cap)
                                ensure_used_capacity(mex);
                        } while (used_tbl[mex]);
                    }
                    break;                  /* move to build next term */
                }
            }
            ++cand;                        /* try next candidate */
        }
    }

    return seq[(size_t)n - 1];
}
