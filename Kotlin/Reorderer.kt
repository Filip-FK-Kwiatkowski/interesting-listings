import java.io.File

fun main() {
    File("datxxxa")
        .walk()
        .maxDepth(1)
        .filter { it.isFile }
        .filter { it.extension == "md"}
        .map { file -> file.useLines { lines -> findOrder(lines) } to file }
        .filter { (order, _ ) -> order != null }
        .sortedBy { (order, _) -> order }
        .map { (_, file) -> file }
        .mapIndexed { index, file -> "${index * 5}" to file }
        .forEach { (index, file) -> replaceOrder(file, index) }
}

private fun findOrder(lines: Sequence<String>): Int? =
    lines.dropWhile { line -> !line.startsWith("order:") }
        .take(1)
        .firstOrNull()
        ?.removePrefix("order: ")
        ?.toInt()

private fun replaceOrder(file: File, index: String) =
    file.writeText(file.readText().replace("order: \\d+".toRegex(), "order: $index"))
