/*
 * A389925  a(1) = 1; for n > 1, a(n) is the smallest unused prime number,
 *          when read in binary, that can be created by either removing or
 *          adding a single 0 or 1 anywhere in the binary value of a(n-1).
 * https://oeis.org/A389925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A389925 – OEIS sequence
 *  a(1) = 1; for n > 1, a(n) is the smallest unused prime that can be
 *  obtained by inserting or deleting a single 0 or 1 anywhere in the binary
 *  representation of a(n‑1).  The sequence is finite (44 terms); subsequent
 *  indices return 0. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"

#define MAXLEN 50          /* enough for the 44-term sequence */

static seq_t seq_arr[MAXLEN];  /* 1‑based indexing */
static idx_t seq_len = 0;      /* number of terms computed so far */
static bool seq_built = false; /* flag that the sequence is ready */

/* check whether a value has already appeared in the sequence */
static bool used_in_seq(seq_t val)
{
    for (idx_t i = 1; i <= seq_len; ++i)
        if (seq_arr[i] == val)
            return true;
    return false;
}

/* build the entire finite sequence once */
static void build_sequence(void)
{
    seq_arr[1] = (seq_t)1;
    seq_len = 1;

    /* loop until no further prime can be found */
    while (true)
    {
        seq_t prev = seq_arr[seq_len];

        /* convert prev to an array of bits (msb first) */
        seq_t temp = prev;
        unsigned char bits[256];           /* msb first */
        size_t len = 0;

        if (temp == (seq_t)0)
        {
            bits[0] = 0;
            len = 1;
        }
        else
        {
            unsigned char rev[256];
            size_t rlen = 0;
            while (temp != (seq_t)0)
            {
                rev[rlen++] = (unsigned char)(temp % (seq_t)2);
                temp /= (seq_t)2;
            }
            /* reverse to msb first */
            for (size_t i = 0; i < rlen; ++i)
                bits[i] = rev[rlen - 1 - i];
            len = rlen;
        }

        seq_t best = 0; /* smallest unused prime found */

        /* candidates by deleting one bit */
        for (size_t del = 0; del < len; ++del)
        {
            seq_t cand = (seq_t)0;
            for (size_t i = 0; i < len; ++i)
            {
                if (i == del)
                    continue;
                cand = cand * (seq_t)2 + (seq_t)bits[i];
            }
            if (isPrimeGeneric(cand) && !used_in_seq(cand))
            {
                if (best == (seq_t)0 || cand < best)
                    best = cand;
            }
        }

        /* candidates by inserting one bit (0 or 1) */
        for (size_t pos = 0; pos <= len; ++pos)
        {
            for (unsigned int newbit = 0; newbit <= 1; ++newbit)
            {
                seq_t cand = (seq_t)0;
                for (size_t i = 0; i < pos; ++i)
                    cand = cand * (seq_t)2 + (seq_t)bits[i];
                cand = cand * (seq_t)2 + (seq_t)newbit;
                for (size_t i = pos; i < len; ++i)
                    cand = cand * (seq_t)2 + (seq_t)bits[i];

                if (isPrimeGeneric(cand) && !used_in_seq(cand))
                {
                    if (best == (seq_t)0 || cand < best)
                        best = cand;
                }
            }
        }

        if (best == (seq_t)0)   /* no more primes reachable */
            break;

        if (seq_len + 1 >= MAXLEN) /* safety guard, should not happen */
            break;

        seq_arr[++seq_len] = best;
    }

    seq_built = true;
}

/*  public function required by the test harness  */
seq_t a(idx_t n)
{
    if (!seq_built)
        build_sequence();

    if (n == 0 || n > seq_len)
        return (seq_t)0;   /* undefined terms report as 0 */
    return seq_arr[n];
}
