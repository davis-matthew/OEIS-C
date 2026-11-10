/*
 * A351847  Arithmetic mean of the distinct prime factors of A078174(n).
 * https://oeis.org/A351847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sopf(A078174(n))/omega(A078174(n)); A078174 found by
 *            incremental search over m >= 2 with omega(m) | sopf(m), results
 *            cached
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-10)
 * Helpers:   factor_uint64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_uint64.h"
#include "../utils/mem.h"

static seq_t *A351847_terms = NULL;
static size_t A351847_size = 0;
static size_t A351847_cap = 0;
static uint64_t A351847_last = 1;

seq_t a(idx_t n)
{
    while (A351847_size < (size_t)n)
    {
        ++A351847_last;
        uint64_t primes[20];
        int ex[20];
        int cnt = 0;
        factor_uint64(A351847_last, primes, ex, &cnt);
        if (cnt <= 0)
            continue;
        unsigned __int128 sum = 0;
        for (int i = 0; i < cnt; ++i)
            sum += primes[i];
        if ((uint64_t)(sum % (unsigned)cnt) == 0)
        {
            if (A351847_size == A351847_cap)
            {
                size_t nc = A351847_cap ? A351847_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, A351847_terms, nc);
                if (!tmp)
                    abort();
                A351847_terms = tmp;
                A351847_cap = nc;
            }
            A351847_terms[A351847_size++] = (seq_t)(uint64_t)(sum / (unsigned)cnt);
        }
    }
    return A351847_terms[n - 1];
}
