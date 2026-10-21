/*
 * A289744  Coefficients in expansion of q*E'_8 where E_8 is the Eisenstein
 *          Series (A008410).
 * https://oeis.org/A289744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 480 * n * sigma_7(n) where sigma_7(n) = Σ_{d|n} d^7
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 118, u128 n <= 10000,
 *            big any n
 * Helpers:   sigma7_of_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sigma7_of_n.h"

/* sequence function ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* build / extend smallest‑prime‑factor table up to n */
    static idx_t spf_len = 0;
    static uint64_t *spf = NULL;

    if (n >= spf_len)
    {
        size_t new_len = (size_t)n + 1;
        uint64_t *tmp_spf = (uint64_t *)realloc(spf, new_len * sizeof(uint64_t));
        if (!tmp_spf) abort();                 /* out of memory */
        spf = tmp_spf;

        for (size_t i = 0; i < new_len; ++i)
            spf[i] = (uint64_t)i;

        for (size_t i = 2; i * i < new_len; ++i)
        {
            if (spf[i] == i)                    /* i is prime */
            {
                for (size_t j = i * i; j < new_len; j += i)
                {
                    if (spf[j] == j)
                        spf[j] = (uint64_t)i;
                }
            }
        }
        spf_len = (idx_t)new_len;
    }

    seq_t sigma7 = sigma7_of_n(n, spf);
    unsigned __int128 tmp = (unsigned __int128)sigma7 *
                            (unsigned __int128)n *
                            (unsigned __int128)480;
    return (seq_t)tmp;
}
