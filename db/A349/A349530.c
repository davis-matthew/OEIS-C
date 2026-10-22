/*
 * A349530  Least positive integer m such that the n numbers k*(k^4+1)
 *          (k=1..n) are pairwise distinct modulo m^2.
 * https://oeis.org/A349530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least positive integer m such that the n numbers k*(k^4+1)
 * (k = 1..n) are pairwise distinct modulo m^2.
 * Computed by testing m = 1,2,3,... and checking distinctness
 * of the residues modulo m^2. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* m = 1 works for n = 1 */

    /* Buffer to mark residues modulo m^2.  Reused for all m. */
    static unsigned char *seen = NULL;
    static size_t seen_cap = 0;

    for (idx_t m = 1; ; ++m)
    {
        /* Size of the residue table: m^2 */
        idx_t m2_idx = m * m;
        size_t sz = (size_t)m2_idx;
        if (sz > seen_cap)
        {
            unsigned char *new_buf = (unsigned char *)realloc(seen, sz);
            if (!new_buf)
                exit(1);          /* allocation failure */
            seen = new_buf;
            seen_cap = sz;
        }

        /* Clear residue table */
        memset(seen, 0, sz);

        /* Modulus m^2 as seq_t */
        seq_t m2 = (seq_t)m * (seq_t)m;

        bool ok = true;
        for (idx_t k = 1; k <= n; ++k)
        {
            seq_t km = (seq_t)k % m2;                 /* k mod m^2 */
            seq_t km2 = (km * km) % m2;              /* k^2 mod m^2 */
            seq_t km4 = (km2 * km2) % m2;            /* k^4 mod m^2 */
            seq_t t  = km4 + (seq_t)1;               /* k^4+1 */
            if (t >= m2)
                t -= m2;                             /* ensure < m^2 */
            seq_t val = (km * t) % m2;              /* k*(k^4+1) mod m^2 */

            /* Index into the residue table */
            size_t idx = (size_t)(unsigned long long)val;
            if (seen[idx])
            {
                ok = false;
                break;                              /* duplicate residue */
            }
            seen[idx] = 1;
        }

        if (ok)
            return (seq_t)m;                          /* first m that works */
    }
}
