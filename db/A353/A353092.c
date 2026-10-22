/*
 * A353092  Inventory sequence counting prime factors. (See comment.)
 * https://oeis.org/A353092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0. For n > 0 let k be a running inventory index
 *            (initially 0). a(n) = number of already existing terms whose
 *            number of prime factors equals k. After appending a(n) the count
 *            of terms with that many prime factors is updated. If a(n) == 0
 *            the inventory index resets to k = 0; otherwise k is i...
 * Range:     u8 n <= 884, u16 n <= 9677, u32 n <= 9677, u64 n <= 9677,
 *            u128 n <= 9677, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"

static seq_t  *seq      = NULL;   /* stored sequence values */
static size_t  seq_cap  = 0;      /* current allocation for seq   */
static idx_t   seq_len  = 0;      /* number of values stored      */

static idx_t  *freq     = NULL;   /* freq[p] = number of terms with p prime factors */
static size_t  freq_cap = 0;      /* current allocation for freq    */
static idx_t   cur_k    = 0;      /* current inventory index        */

static void enlarge_seq(idx_t needed)
{
    if (needed <= (idx_t)seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 64;
    while ((idx_t)new_cap < needed) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void enlarge_freq(idx_t needed)
{
    if (needed <= (idx_t)freq_cap) return;
    size_t new_cap = freq_cap ? freq_cap : 64;
    while ((idx_t)new_cap < needed) new_cap <<= 1;
    idx_t *newp = (idx_t *)realloc(freq, new_cap * sizeof(idx_t));
    for (size_t i = freq_cap; i < new_cap; ++i) newp[i] = 0;
    freq = newp;
    freq_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* one–time initialization */
    if (seq == NULL) {
        seq_cap = 64;
        seq      = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        freq_cap = 64;
        freq     = (idx_t *)malloc(freq_cap * sizeof(idx_t));
        for (size_t i = 0; i < freq_cap; ++i) freq[i] = 0;
        seq_len = 0;
        cur_k   = 0;
    }

    /* generate terms up to index n */
    while (seq_len <= n) {
        /* ensure freq array can index cur_k */
        enlarge_freq((idx_t)cur_k + 1);

        /* a(n) is the current inventory count */
        idx_t cnt = freq[cur_k];
        seq_t val = (seq_t)cnt;

        /* append the new term */
        enlarge_seq(seq_len + 1);
        seq[seq_len++] = val;

        /* determine prime‑factor count of the new term */
        idx_t pf;
        if (val == (seq_t)0) {
            pf = 0;                         /* 0 and 1 have 0 prime factors */
        } else {
            seq_t tmp = bigomega((idx_t)val);/* count multiplicities */
            pf = (idx_t)tmp;
        }

        /* update frequency table for the new term */
        enlarge_freq(pf + 1);
        freq[pf]++;

        /* update inventory index for the next step */
        if (cnt == 0) cur_k = 0;
        else          cur_k++;
    }

    return seq[n];
}
