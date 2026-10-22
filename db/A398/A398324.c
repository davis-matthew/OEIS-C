/*
 * A398324  Positions k such that the prefix of length k of the Kolakoski
 *          sequence contains an even number of 1's and an even number of 2's.
 * https://oeis.org/A398324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = the n‑th even position k of the Kolakoski sequence *
 *            for which both numbers 1 and 2 occur an even number of times *
 *            in the prefix of length k
 * Range:     u8 n <= 63, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n<1, safe fallback */

    /* static state to allow on‑demand, out‑of‑order, repeated calls */
    static uint8_t  *seq   = NULL;      /* stores the Kolakoski sequence (values 1/2) */
    static idx_t    seq_len = 0;        /* current length of seq           */
    static idx_t    seq_cap = 0;       /* allocated capacity of seq      */

    static seq_t   *places     = NULL;  /* positions satisfying the parity */
    static idx_t   places_len  = 0;      /* number of stored positions      */
    static idx_t   places_cap  = 0;      /* allocated capacity of places    */

    /* indices used while generating the sequence */
    static idx_t    run_index  = 1;      /* next index to read as run length */
    static uint8_t  ones_parity = 1;    /* 0 = even #1s so far, 1 = odd #1s  */
                                      /* after seq[0] = 1   */

    /* one‑time initialisation */
    if (seq == NULL) {
        seq_cap  = 1024;
        seq      = (uint8_t*)malloc(seq_cap * sizeof(uint8_t));
        seq[0]   = 1;                   /* first term of Kolakoski seq    */
        seq_len  = 1;
        run_index  = 1;

        places_cap = 64;
        places     = (seq_t*)malloc(places_cap * sizeof(seq_t));
        places_len = 0;
    }

    /* generate terms until we have at least n qualifying positions   */
    while (places_len < n) {
        /* --------------------------------------------------------------------
         * Step 1: Generate the next block of the Kolakoski sequence.
         * The block consists of a run of the alternating value `next_val`
         * whose length is determined by the current run length.
         * -------------------------------------------------------------------- */

        /* produce the next alternating value */
        uint8_t next_val = 3 - seq[seq_len-1];
        /* extend the array if necessary */
        if (seq_len >= seq_cap) {
            seq_cap <<= 1;
            seq = (uint8_t*)realloc(seq, seq_cap * sizeof(uint8_t));
        }
        seq[seq_len++] = next_val;

        /* update parity of 1's */
        if (next_val == 1) ones_parity ^= 1;

        /* check if the current prefix satisfies the condition */
        if ((seq_len & 1U) == 0 && ones_parity == 0) {
            if (places_len >= places_cap) {
                places_cap <<= 1;
                places = (seq_t*)realloc(places,
                                         places_cap * sizeof(seq_t));
            }
            places[places_len++] = (seq_t)seq_len;
        }

        /* --------------------------------------------------------------------
         * Step 2: Duplicate the value to complete the run.
         * The run length is obtained from seq[run_index].
         * -------------------------------------------------------------------- */

        /* obtain run length; seq[run_index] is guaranteed to exist */
        uint8_t run_len = seq[run_index++];
        /* the first element of the run was just written, need run_len-1 more */
        for (idx_t j = 1; j < (idx_t)run_len; ++j) {
            if (seq_len >= seq_cap) {
                seq_cap <<= 1;
                seq = (uint8_t*)realloc(seq, seq_cap * sizeof(uint8_t));
            }
            seq[seq_len++] = next_val;
            if (next_val == 1) ones_parity ^= 1;
            if ((seq_len & 1U) == 0 && ones_parity == 0) {
                if (places_len >= places_cap) {
                    places_cap <<= 1;
                    places = (seq_t*)realloc(places,
                                             places_cap * sizeof(seq_t));
                }
                places[places_len++] = (seq_t)seq_len;
            }
        }
    }

    /* return the requested element (1‑based indexing) */
    return places[n-1];
}
