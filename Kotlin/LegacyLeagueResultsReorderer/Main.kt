import java.io.File

class PlayerRecord(
    private val nameAndSurname: String,
    private val scores: List<Int?> = listOf(),
) : Comparable<PlayerRecord> {
    private val totalPoints: Int = scores.filterNotNull().sum()

    override fun compareTo(other: PlayerRecord): Int {
        return other.totalPoints.compareTo(totalPoints)
    }

    override fun toString(): String {
        val formattedPoints = scores.joinToString(separator = " | ") { it?.toString().orEmpty() }

        return "| $nameAndSurname | $$ $totalPoints $$ | $formattedPoints |"
    }

    companion object {
        fun fromLine(line: String): PlayerRecord {
            val rawPoints = line.split("|").drop(4).dropLast(1)

            val nameAndSurname = line.split("|")[2].trim()
            val points = rawPoints.map { it.trim().toIntOrNull() }

            val result = PlayerRecord(nameAndSurname, points.toMutableList())
            return result
        }
    }
}

fun main() {
    val path = "path/to/your/file.md"
    val playerRecords = File(path).useLines { lines ->
        lines
            .filter { it.startsWith('|') }
            .drop(2)
            .map(PlayerRecord::fromLine)
            .toMutableList()
    }

    playerRecords.sort()

    for ((index, player) in playerRecords.withIndex()) {
        println("| $$ ${index + 1}. $$ $player")
    }
}
