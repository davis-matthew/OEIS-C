/*
 * A370046  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest unused
 *          positive number whose binary value is a substring of the binary
 *          value of the sum of all previous terms.
 * https://oeis.org/A370046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) defined by recurrence a(1) = 1, a(2) = 2. for n > 2: a(n)
 *            is the smallest unused positive integer whose binary
 *            representation occurs as a substring of the binary
 *            representation of the sum of all previous terms
 * Range:     u8 n <= 22, u16 n <= 387, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper to compute the bit length of a seq_t value. */
static unsigned bitlen_seq_t_local(seq_t x)
{
    unsigned len = 0;
    while (x) {
        len++;
        x >>= 1;
    }
    return len;
}

/* ----------  storage for computed sequence  ---------- */
static seq_t *seq_arr   = NULL;   /* array of a(k) values */
static size_t seq_len   = 0;      /* number of terms stored */
static size_t seq_cap   = 0;      /* allocated capacity */
static seq_t  sum_so_far = 0;     /* sum of all computed terms */

/* ----------  tracking used integers  ---------- */
static bool *used_arr   = NULL;   /* indexed boolean array for small values */
static size_t used_limit = 0;     /* size of used_arr (number of indices) */

static seq_t *big_used   = NULL;  /* list of used values that do not fit in size_t */
static size_t big_used_len = 0;
static size_t big_used_cap = 0;

/* Ensure storage arrays are large enough. */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap*2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *p = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!p) exit(1);
    seq_arr = p;
    seq_cap = new_cap;
}

static void ensure_used_arr(size_t new_limit)
{
    if (new_limit <= used_limit) return;
    bool *p = (bool*)realloc(used_arr, new_limit * sizeof(bool));
    if (!p) exit(1);
    for (size_t i = used_limit; i < new_limit; ++i) p[i] = false;
    used_arr = p;
    used_limit = new_limit;
}

/* ---- functions to query and set usage status ---- */
static bool value_used(seq_t v)
{
    unsigned bits = bitlen_seq_t_local(v);
    if (bits >= 8 * sizeof(size_t)) {
        for (size_t i = 0; i < big_used_len; ++i)
            if (big_used[i] == v) return true;
        return false;
    }
    size_t idx = (size_t)v;
    if (idx >= used_limit) return false;
    return used_arr[idx];
}

static void mark_used(seq_t v)
{
    unsigned bits = bitlen_seq_t_local(v);
    if (bits >= 8 * sizeof(size_t)) {
        if (big_used_len == big_used_cap) {
            size_t new_cap = big_used_cap ? 2*big_used_cap : 8;
            seq_t *p = (seq_t*)realloc(big_used, new_cap * sizeof(seq_t));
            if (!p) exit(1);
            big_used = p;
            big_used_cap = new_cap;
        }
        big_used[big_used_len++] = v;
        return;
    }
    size_t idx = (size_t)v;
    if (idx >= used_limit) {
        size_t new_limit = idx + 1;
        ensure_used_arr(new_limit);
    }
    used_arr[idx] = true;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not part of the sequence, but harmless */

    /* Compute terms up to n */
    while (seq_len < (size_t)n) {
        seq_t val;

        if (seq_len == 0) {
            val = (seq_t)1;
        } else if (seq_len == 1) {
            val = (seq_t)2;
        } else {
            /* Compute bit representation of the current sum (LSB first) */
            seq_t tmp = sum_so_far;
            size_t bitlen = 0;
            while (tmp) { tmp >>= 1; bitlen++; }

            unsigned char *bits = (unsigned char*)malloc(bitlen * sizeof(unsigned char));
            if (!bits) exit(1);
            tmp = sum_so_far;
            for (size_t i = 0; i < bitlen; ++i) {
                bits[i] = (unsigned char)(tmp & 1);
                tmp >>= 1;
            }

            /* Find the smallest unused integer whose binary is a substring
               of the binary of sum_so_far. */
            bool found = false;
            seq_t best = 0;

            for (size_t start = 0; start < bitlen; ++start) {
                unsigned char msb = bits[bitlen - 1 - start];
                if (msb == 0) continue; /* substring must start with '1' */

                seq_t cand = 0;
                for (size_t end = start; end < bitlen; ++end) {
                    unsigned char b = bits[bitlen - 1 - end];
                    cand = (cand << 1) | b;
                    if (!value_used(cand)) {
                        if (!found || cand < best) {
                            best = cand;
                            found = true;
                        }
                    }
                }
            }

            free(bits);
            val = best;
        }

        /* Record the term and update state */
        mark_used(val);
        ensure_seq_cap(seq_len + 1);
        seq_arr[seq_len++] = val;
        sum_so_far += val;
    }

    return seq_arr[n - 1];
}
