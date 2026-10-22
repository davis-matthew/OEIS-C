/*
 * A323398  Lexicographically first 3-free sequence on nonnegative integers
 *          not containing the Stanley sequence S(0,1), which is A005836.
 * https://oeis.org/A323398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   ensure_bitset_size, is_stanley
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A323398:
   Lexicographically first 3‑free sequence on nonnegative integers
   that does NOT contain the Stanley sequence S(0,1).  The sequence is
   generated greedily: starting from 0, scan increasing integers and
   add the smallest candidate that
       1) is not in S(0,1)  (i.e. its base‑3 representation contains a 2),
       2) is not the middle of an arithmetic progression of two chosen
          terms (i.e. 2*c is not the sum of two chosen terms), and
       3) does not become the right endpoint of an arithmetic
          progression whose middle element already belongs to the
          sequence (i.e. c is not 2*m - a for any earlier a < m). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ensure_bitset_size.h"
#include "../utils/is_stanley.h"

/* Simple bitset helpers ------------------------------------------- */
static inline void set_bit(uint64_t *arr, size_t idx)
{
    arr[idx >> 6] |= (uint64_t)1ULL << (idx & 63);
}

static inline bool get_bit(const uint64_t *arr, size_t idx)
{
    return (arr[idx >> 6] >> (idx & 63)) & 1ULL;
}

/* State kept between calls ------------------------------------------ */
static seq_t *seq_store  = NULL;   /* array of chosen terms      */
static size_t seq_cnt    = 0;
static size_t seq_cap    = 0;

static uint64_t *chosen_bits   = NULL;   /* marks chosen numbers   */
static size_t   chosen_len    = 0;

static uint64_t *sum_bits      = NULL;   /* marks x+y of chosen    */
static size_t   sum_len       = 0;

static uint64_t *forbidden_bits = NULL;   /* marks forbidden endpoints  */
static size_t   forbidden_len  = 0;

static idx_t next_candidate = 0;          /* next integer to test   */

/* Append a new term t to the sequence (t is known to be acceptable). */
static void add_term(idx_t t)
{
    /* Ensure bitsets large enough for indices up to 2*t. */
    ensure_bitset_size(&chosen_bits,   &chosen_len,   t);
    ensure_bitset_size(&sum_bits,      &sum_len,      2*t);
    ensure_bitset_size(&forbidden_bits,&forbidden_len,2*t);

    /* Mark t as chosen. */
    set_bit(chosen_bits, t);

    /* Store the term. */
    if (seq_cnt == seq_cap) {
        size_t newcap = seq_cap ? seq_cap * 2 : 8;
        seq_t *tmp = (seq_t*)realloc(seq_store, newcap * sizeof(seq_t));
        if (!tmp) abort();
        seq_store = tmp;
        seq_cap   = newcap;
    }
    seq_store[seq_cnt++] = (seq_t)t;

    /* Update data structures using all previously chosen elements. */
    for (size_t i = 0; i < seq_cnt - 1; ++i) {
        idx_t a = (idx_t)seq_store[i];
        set_bit(sum_bits,        a + t);        /* t + a   */
        set_bit(forbidden_bits,  2*t - a);      /* 2t - a  */
    }
    /* Self‑sum (t + t) also belongs to sumset. */
    set_bit(sum_bits, 2*t);
}

/* Public sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */

    /* Generate terms until the requested index is reached. */
    while (seq_cnt < (size_t)n) {
        /* Make sure the bitsets can hold bits for this candidate. */
        ensure_bitset_size(&sum_bits,      &sum_len,      2*next_candidate);
        ensure_bitset_size(&forbidden_bits,&forbidden_len, next_candidate);

        /* Skip numbers that lie in the Stanley sequence S(0,1). */
        if (!is_stanley((seq_t)next_candidate)) {
            /* Reject if it would become the middle of an AP
               or the right endpoint of an AP whose middle is already chosen. */
            if (!get_bit(sum_bits, 2*next_candidate) &&
                !get_bit(forbidden_bits, next_candidate)) {
                add_term(next_candidate);
            }
        }
        ++next_candidate;
    }

    return seq_store[n-1];
}
